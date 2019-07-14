#include "CodeDecorator.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/VarNode.h"

#include "decorator/Decorator.h"

CodeDecorator::CodeDecorator(Context &c) : c(c)
{
}

void CodeDecorator::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n.accept(*this);
    }
}

void CodeDecorator::visit(ScopeNode &node)
{
    auto sym = c.tree.current()->add(new Sym(Sym::Type::Scope, node.location(), { }));
    node.setProperty("sym", sym);

    auto g = c.tree.open(sym);
    node.body->accept(*this);
}

void CodeDecorator::visit(VarNode &node)
{
    Decorator d(c);
    node.accept(d);
}
