#include "CodeDecorator.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/VarNode.h"
#include "nodes/ExprNode.h"
#include "nodes/ReturnNode.h"

#include "decorator/Decorator.h"
#include "decorator/ExprDecorator.h"

#include "types/Type.h"

CodeDecorator::CodeDecorator(Context &c) : c(c)
{
}

void CodeDecorator::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
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

void CodeDecorator::visit(ExprNode &node)
{
    ExprDecorator::decorate(c, nullptr, *node.expr);
}

void CodeDecorator::visit(ReturnNode &node)
{
    ExprDecorator::decorate(c, c.tree.current()->container()->property<const Type*>("type")->returnType, *node.expr);
}
