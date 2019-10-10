#include "Transform.h"

#include "application/Context.h"
#include "application/Pragmas.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ClassNode.h"
#include "nodes/IdNode.h"
#include "nodes/VarNode.h"
#include "nodes/InitNode.h"

#include "decorator/FuncDecorator.h"

#include "transform/FuncTransform.h"
#include "transform/ExprTransform.h"

#include "types/Type.h"

#include "visitors/TypeVisitor.h"
#include "visitors/QueryVisitors.h"

namespace
{

void generateInitialisers(Context &c, FuncNode &node, Sym *sym)
{
    std::unordered_map<std::string, NodePtr> map;
    for(auto &i: node.inits)
    {
        Visitor::visit<QueryVisitors::InitNodeMap>(i.get(), map, i);
    }

    auto block = Visitor::query<QueryVisitors::GetBlockNode, BlockNode*>(&node);
    std::size_t insert = 0;

    for(auto s: sym->children())
    {
        if(s->type() == Sym::Type::Var && !s->findProperty("free").value<bool>())
        {
            NodePtr n;

            auto i = map.find(s->name());
            if(i != map.end())
            {
                n = i->second;
            }

            if(!n)
            {
                auto t = s->property<Type*>("type");

                if(t->ref)
                {
                    throw Error(node.location(), "reference must be initialised - ", s->fullname());
                }

                if(!t->primitive())
                {
                    n = new InitNode(node.location(), s->name());
                    Visitor::visit<FuncDecorator>(n.get(), c);
                }
            }

            if(n)
            {
                block->insert(insert++, n);
            }
        }
    }

    node.inits.clear();
}

}

Transform::Transform(Context &c) : c(c)
{
}

void Transform::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void Transform::visit(NamespaceNode &node)
{
    auto sg = c.tree.open(node.property<Sym*>("sym"));
    node.body->accept(*this);
}

void Transform::visit(FuncNode &node)
{
    auto type = node.property<Sym*>("sym")->property<Type*>("type");

    if(type->method)
    {
        NodePtr name(new IdNode(node.location(), { }, "this"));

        auto vn = new VarNode(node.location(), name);
        node.args.insert(node.args.begin(), vn);

        auto sym = node.property<Sym*>("sym");

        auto t = Type::makePrimary(sym->parent());

        t.ref = true;
        t.constant = type->constMethod;

        auto tp = c.types.insert(t);

        Sym *s = sym->child("this");
        if(!s)
        {
            s = sym->insert(0, new Sym(Sym::Type::Var, node.location(), Access::Private, "this"));
            s->setProperty("type", tp);
        }

        vn->setProperty("sym", s);

        if(!sym->findProperty("thistransformed").value<bool>())
        {
            auto t = *type;
            t.args.insert(t.args.begin(), tp);

            auto p = c.types.insert(t);

            sym->setProperty("type", p);
            sym->setProperty("thistransformed", true);
        }

        if(sym->name() == "new")
        {
            generateInitialisers(c, node, sym->parent());
        }
    }

    if(node.body)
    {
        auto sg = c.tree.open(node.property<Sym*>("sym"));

        Visitor::visit<FuncTransform>(node.body.get(), c);
    }
}

void Transform::visit(ClassNode &node)
{
    if(node.body)
    {
        auto sg = c.tree.open(node.property<Sym*>("sym"));
        node.body->accept(*this);
    }
}

void Transform::visit(VarNode &node)
{
    if(node.value && !node.findProperty("globalinit").value<bool>())
    {
        node.value = ExprTransform::transform(c, node.value);
    }
}

void Transform::visit(PragmaNode &node)
{
    Pragmas::execute(c, node);
}
