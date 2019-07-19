#include "Transformer.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/FuncNode.h"

#include "transform/FuncTransformer.h"

Transformer::Transformer(Context &c) : c(c)
{
}

void Transformer::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void Transformer::visit(NamespaceNode &node)
{
    auto g = c.tree.open(node.property<Sym*>("sym"));
    node.body->accept(*this);
}

void Transformer::visit(FuncNode &node)
{
    if(node.body)
    {
        auto g = c.tree.open(node.property<Sym*>("sym"));

        FuncTransformer ft(c);
        node.body->accept(ft);
    }
}
