#include "ExprDecorator.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"

#include "visitors/SymFinder.h"

ExprDecorator::ExprDecorator(Context &c) : c(c)
{
}

void ExprDecorator::visit(IdNode &node)
{
    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Global, c.tree.current(), &node, sv);

    node.setProperty("syms", sv);
}

void ExprDecorator::visit(IntLiteralNode &node)
{
    node.setProperty("type", c.types.intType());
}

void ExprDecorator::visit(CallNode &node)
{
    node.target->accept(*this);

    for(auto &p: node.params)
    {
        p->accept(*this);
    }
}
