#include "FuncDecorator.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/VarNode.h"
#include "nodes/ExprNode.h"
#include "nodes/ReturnNode.h"

#include "decorator/VarDecorator.h"
#include "decorator/ExprDecorator.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include "visitors/TypeVisitor.h"

#include "framework/Trace.h"

FuncDecorator::FuncDecorator(Context &c) : c(c)
{
}

void FuncDecorator::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void FuncDecorator::visit(ScopeNode &node)
{
    auto sym = c.tree.current()->add(new Sym(Sym::Type::Scope, node.location(), pcx::str("#scope", c.func().scopes++)));
    node.setProperty("sym", sym);

    auto sg = c.tree.open(sym);
    node.body->accept(*this);
}

void FuncDecorator::visit(VarNode &node)
{
    Visitor::visit<VarDecorator>(&node, c);
}

void FuncDecorator::visit(ExprNode &node)
{
    node.expr = ExprDecorator::decorate(c, node.expr);
}

void FuncDecorator::visit(ReturnNode &node)
{
    auto t = c.tree.current()->container()->property<Type*>("type")->returnType;

    if(node.expr)
    {
        node.expr = ExprDecorator::decorate(c, node.expr, t);

        if(!TypeCompare(c).compatible(t, TypeVisitor::assertType(c, node.expr.get())))
        {
            throw Error(node.expr->location(), t->text(), " expected - ", node.expr->description());
        }
    }
    else if(!TypeCompare(c).compatible(t, c.types.nullType()))
    {
        throw Error(node.location(), "function must return ", t->text());
    }

    auto s = c.tree.current();
    std::size_t n = 0;

    while(s && s != c.tree.current()->container())
    {
        s = s->parent();
        ++n;
    }

    if(n == 1)
    {
        c.tree.current()->container()->setProperty("returned", true);
    }
}
