#include "DefaultMethods.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/TypeNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"
#include "nodes/InitNode.h"

#include "syms/Sym.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include "decorator/Decorator.h"
#include "decorator/FuncDecorator.h"

#include "visitors/QueryVisitors.h"

namespace
{

FuncNode *createBasicFunction(Sym *sym, BlockNode *block, const std::string &name)
{
    NodePtr n(new IdNode(block->location(), { }, name));

    auto fn = new FuncNode(block->location(), n);
    block->push_back(fn);

    fn->autoGen = true;

    auto sc = new ScopeNode(block->location());
    fn->body = sc;

    auto body = new BlockNode(block->location());
    sc->body = body;

    return fn;
}

bool hasCopyMethod(Context &c, Sym *sym)
{
    auto t = Type::makeFunction(c.types.nullType(), { c.types.insert(Type::makePrimary(sym)) });

    for(auto s: sym->children())
    {
        if(s->name() == "new")
        {
            if(TypeCompare(c).compatibleArgs(s->property<Type*>("type"), &t))
            {
                return true;
            }
        }
    }

    return false;
}

}

void DefaultMethods::generate(Context &c, ClassNode &node, Sym *sym)
{
    auto block = Visitor::query<QueryVisitors::GetBlockNode, BlockNode*>(&node);

    if(!sym->child("new"))
    {
        Visitor::visit<Decorator>(createBasicFunction(sym, block, "new"), c);
    }

    if(!hasCopyMethod(c, sym))
    {
        FuncNode *fn = createBasicFunction(sym, block, "new");

        NodePtr pn = new IdNode(node.location(), { }, "#tempcopy");

        auto vn = new VarNode(node.location(), pn);
        fn->args.push_back(vn);

        auto tn = new TypeNode(node.location());
        vn->type = tn;

        tn->name = new IdNode(node.location(), { }, sym->name());
        tn->constant = true;
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

        Visitor::visit<Decorator>(fn, c);
    }

    if(!sym->child("delete"))
    {
        Visitor::visit<Decorator>(createBasicFunction(sym, block, "delete"), c);
    }
}
