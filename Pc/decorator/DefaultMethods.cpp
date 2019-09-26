#include "DefaultMethods.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ClassNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include "decorator/Decorator.h"

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


}

void DefaultMethods::generate(Context &c, ClassNode &node, Sym *sym)
{
    auto block = Visitor::query<QueryVisitors::GetBlockNode, BlockNode*>(&node);

    if(!sym->child("new"))
    {
        Visitor::visit<Decorator>(createBasicFunction(sym, block, "new"), c);
    }

    if(!sym->child("delete"))
    {
        Visitor::visit<Decorator>(createBasicFunction(sym, block, "delete"), c);
    }
}
