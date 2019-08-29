#include "ClassMethodDecorator.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/IdNode.h"
#include "nodes/VarNode.h"
#include "nodes/TypeNode.h"
#include "nodes/FuncNode.h"
#include "nodes/InitNode.h"

#include "decorator/ClassDecorator.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

namespace
{

FuncNode *createBasicFunction(Sym *sym, BlockNode *block, const std::string &name)
{
    NodePtr nn(new IdNode(block->location(), { }, name));

    auto fn = new FuncNode(block->location(), nn);
    block->push_back(fn);

    auto sc = new ScopeNode(block->location());
    fn->body = sc;

    sc->body = new BlockNode(block->location());

    return fn;
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
        fn->accept(cd);
    }

    if(!hasNewCopyMethod(sym))
    {
        auto fn = createBasicFunction(sym, &node, "new");

        NodePtr pn = new IdNode(node.location(), { }, "#tempcopy");

        auto vn = new VarNode(node.location(), pn);
        fn->args.push_back(vn);

        auto tn = new TypeNode(node.location());
        vn->type = tn;

        tn->name = new IdNode(node.location(), { }, sym->name());
        tn->ref = true;

        for(auto s: sym->children())
        {
            if(s->type() == Sym::Type::Var)
            {
                auto in = new InitNode(node.location(), s->name());
                fn->inits.push_back(in);

                in->params.push_back(new IdNode(node.location(), pn, s->name()));
            }
        }

        ClassDecorator cd(c);
        fn->accept(cd);
    }

    if(!sym->child("delete"))
    {
        auto fn = createBasicFunction(sym, &node, "delete");

        ClassDecorator cd(c);
        fn->accept(cd);
    }
}
