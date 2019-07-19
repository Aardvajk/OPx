#include "FuncTransformer.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/VarNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ExprNode.h"
#include "nodes/CallNode.h"
#include "nodes/AddrOfNode.h"

#include "visitors/NameVisitors.h"

#include "transform/ExprTransformer.h"

#include "decorator/ExprDecorator.h"

FuncTransformer::FuncTransformer(Context &c) : c(c), index(0)
{
}

void FuncTransformer::visit(BlockNode &node)
{
    for(std::size_t i = 0; i < node.nodes.size(); ++i)
    {
        index = i;
        node.nodes[i]->accept(*this);
    }
}

void FuncTransformer::visit(VarNode &node)
{
    if(node.value)
    {
        auto en = new ExprNode(node.location(), { });
        node.block()->insert(index + 1, en);

        auto cn = new CallNode(node.location(), new IdNode(node.location(), "operator="));
        en->expr = cn;

        cn->params.push_back(new AddrOfNode(node.location(), node.name));
        cn->params.push_back(node.value);

        ExprDecorator::decorate(c, nullptr, *cn);
    }
}

void FuncTransformer::visit(ScopeNode &node)
{
    auto g = c.tree.open(node.property<Sym*>("sym"));
    node.body->accept(*this);
}

void FuncTransformer::visit(ExprNode &node)
{
    node.expr = ExprTransformer::transform(c, node.expr);
}
