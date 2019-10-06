#include "Convert.h"

#include "application/Context.h"
#include "application/Pragmas.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"

#include "convert/FuncConvert.h"
#include "convert/ExprConvert.h"

#include "visitors/TypeVisitor.h"

Convert::Convert(Context &c) : c(c)
{
}

void Convert::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void Convert::visit(NamespaceNode &node)
{
    auto sg = c.tree.open(node.property<Sym*>("sym"));
    node.body->accept(*this);
}

void Convert::visit(FuncNode &node)
{
    if(node.body)
    {
        auto sg = c.tree.open(node.property<Sym*>("sym"));
        Visitor::visit<FuncConvert>(node.body.get(), c);
    }
}

void Convert::visit(ClassNode &node)
{
    if(node.body)
    {
        auto sg = c.tree.open(node.property<Sym*>("sym"));
        node.body->accept(*this);
    }
}

void Convert::visit(VarNode &node)
{
    if(node.value)
    {
        node.value = ExprConvert::convert(c, node.value, TypeVisitor::assertType(c, &node));
    }
}

void Convert::visit(PragmaNode &node)
{
    Pragmas::execute(c, node);
}
