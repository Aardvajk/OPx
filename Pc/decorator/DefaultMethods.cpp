#include "DefaultMethods.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/TypeNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"
#include "nodes/ExprNode.h"
#include "nodes/AssignNode.h"
#include "nodes/InitNode.h"
#include "nodes/ReturnNode.h"
#include "nodes/ThisNode.h"

#include "syms/Sym.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include "decorator/Decorator.h"
#include "decorator/FuncDecorator.h"

#include "visitors/QueryVisitors.h"

namespace
{

FuncNode *createBasicFunction(BlockNode *block, const std::string &name, std::size_t index)
{
    NodePtr n(new IdNode(block->location(), { }, name));

    auto fn = new FuncNode(block->location(), n);
    block->insert(index, fn);

    fn->setProperty("access", Access::Public);
    fn->setProperty("autogen", true);

    auto sc = new ScopeNode(block->location());
    fn->body = sc;

    auto body = new BlockNode(block->location());
    sc->body = body;

    return fn;
}

bool hasCopyMethod(Context &c, const std::string &name, Sym *sym)
{
    auto t = Type::makeFunction(c.types.nullType(), { c.types.insert(Type::makePrimary(sym)) });

    for(auto s: sym->children())
    {
        if(s->name() == name)
        {
            auto args = s->property<Type*>("type")->args;

            auto no = s->findProperty("defaults").value<std::size_t>();
            while(no)
            {
                args.pop_back();
                --no;
            }

            if(TypeCompare(c).compatibleArgs(args, t.args))
            {
                return true;
            }
        }
    }

    return false;
}

bool anyConstOrRefMembers(Sym *sym)
{
    for(auto s: sym->children())
    {
        if(s->type() == Sym::Type::Var)
        {
            auto t = s->property<Type*>("type");
            if(t->constant || t->ref)
            {
                return true;
            }
        }
    }

    return false;
}

NodePtr makeType(Node &node, bool constant, Sym *sym)
{
    auto tn = new TypeNode(node.location());
    NodePtr n(tn);

    tn->name = new IdNode(node.location(), { }, sym->name());
    tn->constant = constant;
    tn->ref = true;

    return n;
}

NodePtr makeParam(Node &node, FuncNode *fn, Sym *sym)
{
    NodePtr pn = new IdNode(node.location(), { }, "#tempcopy");

    auto vn = new VarNode(node.location(), pn);
    fn->args.push_back(vn);

    vn->setProperty("access", Access::Private);
    vn->type = makeType(node, true, sym);

    return pn;
}

}

void DefaultMethods::generate(Context &c, ClassNode &node, Sym *sym)
{
    auto block = Visitor::query<QueryVisitors::GetBlockNode, BlockNode*>(&node);
    std::size_t index = 0;

    if(!sym->child("new"))
    {
        createBasicFunction(block, "new", index);
        Decorator::decorate(c, block->nodes[index++]);
    }

    if(!hasCopyMethod(c, "new", sym))
    {
        FuncNode *fn = createBasicFunction(block, "new", index);
        auto pn = makeParam(node, fn, sym);

        for(auto s: sym->children())
        {
            if(s->type() == Sym::Type::Var && !s->findProperty("free").value<bool>())
            {
                auto in = new InitNode(node.location(), s->name());
                fn->inits.push_back(in);

                in->params.push_back(new IdNode(node.location(), pn, s->name()));
            }
        }

        Decorator::decorate(c, block->nodes[index++]);
    }

    if(!sym->child("delete"))
    {
        createBasicFunction(block, "delete", index);
        Decorator::decorate(c, block->nodes[index++]);
    }

    if(!hasCopyMethod(c, "operator=", sym) && !anyConstOrRefMembers(sym))
    {
        FuncNode *fn = createBasicFunction(block, "operator=", index);
        fn->type = makeType(node, false, sym);

        auto pn = makeParam(node, fn, sym);

        auto fb = Visitor::query<QueryVisitors::GetBlockNode, BlockNode*>(fn);

        for(auto s: sym->children())
        {
            if(s->type() == Sym::Type::Var && !s->findProperty("free").value<bool>())
            {
                auto en = new ExprNode(node.location());
                fb->push_back(en);

                NodePtr tn(new IdNode(node.location(), { }, s->name()));

                auto an = new AssignNode(node.location(), tn);
                en->expr = an;

                an->expr = new IdNode(node.location(), pn, s->name());
            }
        }

        auto rn = new ReturnNode(node.location());
        fb->push_back(rn);

        rn->expr = new ThisNode(node.location());

        Decorator::decorate(c, block->nodes[index++]);
    }
}

