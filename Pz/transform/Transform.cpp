#include "Transform.h"

#include "application/Context.h"
#include "application/Pragmas.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ClassNode.h"
#include "nodes/IdNode.h"
#include "nodes/VarNode.h"

#include "transform/FuncTransform.h"

#include "types/Type.h"

#include "visitors/TypeVisitor.h"

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

        auto s = sym->insert(0, new Sym(Sym::Type::Var, node.location(), "this"));
        s->setProperty("type", tp);

        vn->setProperty("sym", s);

        if(!sym->findProperty("thistransformed").value<bool>())
        {
            auto t = *type;
            t.args.insert(t.args.begin(), tp);

            auto p = c.types.insert(t);

            sym->setProperty("type", p);
            sym->setProperty("thistransformed", true);
        }
    }

    if(node.body)
    {
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

void Transform::visit(PragmaNode &node)
{
    Pragmas::execute(c, node);
}
