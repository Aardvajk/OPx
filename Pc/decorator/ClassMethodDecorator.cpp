#include "ClassMethodDecorator.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/IdNode.h"
#include "nodes/VarNode.h"
#include "nodes/TypeNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ExprNode.h"
#include "nodes/AssignNode.h"
#include "nodes/InitNode.h"
#include "nodes/ReturnNode.h"
#include "nodes/ThisNode.h"

#include "decorator/ClassDecorator.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

namespace
{

std::pair<FuncNode*, BlockNode*> createBasicFunction(Sym *sym, BlockNode *block, const std::string &name)
{
    NodePtr nn(new IdNode(block->location(), { }, name));

    auto fn = new FuncNode(block->location(), nn);
    block->push_back(fn);

    fn->flags |= Object::Entity::Flag::AutoGen;

    auto sc = new ScopeNode(block->location());
    fn->body = sc;

    auto bn = new BlockNode(block->location());
    sc->body = bn;

    return std::make_pair(fn, bn);
}

bool hasNewCopyMethod(Sym *sym)
{
    auto pt = Type::makePrimary(0, sym);
    pt.ref = true;

    for(auto s: sym->children())
    {
        if(s->name() == "new")
        {
            auto t = s->property<const Type*>("type");

            if(t->args.size() == 1 && TypeCompare::compatible(&pt, t->args[0]))
            {
                return true;
            }
        }
    }

    return false;
}

bool anyRefMembers(Sym *sym)
{
    for(auto s: sym->children())
    {
        if(s->type() == Sym::Type::Var)
        {
            if(s->property<const Type*>("type")->ref)
            {
                return true;
            }
        }
    }

    return false;
}

}

ClassMethodDecorator::ClassMethodDecorator(Context &c, Sym *sym) : c(c), sym(sym)
{
}

void ClassMethodDecorator::visit(BlockNode &node)
{
    if(!sym->child("new"))
    {
        auto fn = createBasicFunction(sym, &node, "new");

        ClassDecorator cd(c);
        fn.first->accept(cd);
    }

    if(!hasNewCopyMethod(sym))
    {
        auto fn = createBasicFunction(sym, &node, "new");

        NodePtr pn = new IdNode(node.location(), { }, "#tempcopy");

        auto vn = new VarNode(node.location(), pn);
        fn.first->args.push_back(vn);

        auto tn = new TypeNode(node.location());
        vn->type = tn;

        tn->name = new IdNode(node.location(), { }, sym->name());
        tn->ref = true;

        for(auto s: sym->children())
        {
            if(s->type() == Sym::Type::Var)
            {
                auto in = new InitNode(node.location(), s->name());
                fn.first->inits.push_back(in);

                in->params.push_back(new IdNode(node.location(), pn, s->name()));
            }
        }

        ClassDecorator cd(c);
        fn.first->accept(cd);
    }

    if(!sym->child("delete"))
    {
        auto fn = createBasicFunction(sym, &node, "delete");

        ClassDecorator cd(c);
        fn.first->accept(cd);
    }

    if(!sym->child("operator=") && !anyRefMembers(sym))
    {
        auto fn = createBasicFunction(sym, &node, "operator=");

        NodePtr pn = new IdNode(node.location(), { }, "#tempcopy");

        auto tn = new TypeNode(node.location());
        NodePtr tnn(tn);

        fn.first->type = tnn;

        tn->name = new IdNode(node.location(), { }, sym->name());
        tn->ref = true;

        auto vn = new VarNode(node.location(), pn);
        fn.first->args.push_back(vn);

        vn->type = tnn;

        for(auto s: sym->children())
        {
            if(s->type() == Sym::Type::Var)
            {
                auto en = new ExprNode(node.location(), { });
                fn.second->push_back(en);

                NodePtr tn(new IdNode(node.location(), { }, s->name()));

                auto an = new AssignNode(node.location(), tn);
                en->expr = an;

                an->expr = new IdNode(node.location(), pn, s->name());
            }
        }

        auto rn = new ReturnNode(node.location());
        fn.second->push_back(rn);

        rn->expr = new ThisNode(node.location());

        ClassDecorator cd(c);
        fn.first->accept(cd);
    }
}
