#include "FuncDecorator.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/VarNode.h"
#include "nodes/ExprNode.h"
#include "nodes/ReturnNode.h"
#include "nodes/WhileNode.h"

#include "visitors/TypeVisitor.h"
#include "visitors/NameVisitors.h"

#include "decorator/Decorator.h"
#include "decorator/ExprDecorator.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

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
    auto sym = c.tree.current()->add(new Sym(Sym::Type::Scope, node.location(), { }));
    node.setProperty("sym", sym);

    auto g = c.tree.open(sym);
    node.body->accept(*this);
}

void FuncDecorator::visit(VarNode &node)
{
    Decorator d(c);
    node.accept(d);
}

void FuncDecorator::visit(ExprNode &node)
{
    ExprDecorator::decorate(c, nullptr, *node.expr);
}

void FuncDecorator::visit(ReturnNode &node)
{
    auto rt = c.tree.current()->container()->property<const Type*>("type")->returnType;

    ExprDecorator::decorate(c, rt, *node.expr);

    if(!TypeCompare::exact(TypeVisitor::type(c, node.expr.get()), rt))
    {
        throw Error(node.expr->location(), rt->text(), " expected - ", NameVisitors::prettyName(node.expr.get()));
    }
}

void FuncDecorator::visit(WhileNode &node)
{
    ExprDecorator::decorate(c, nullptr, *node.expr);
    node.body->accept(*this);
}
