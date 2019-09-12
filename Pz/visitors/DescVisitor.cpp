#include "DescVisitor.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/TypeNode.h"
#include "nodes/VarNode.h"

DescVisitor::DescVisitor()
{
}

void DescVisitor::visit(BlockNode &node)
{
    r += "(block)";
}

void DescVisitor::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent->accept(*this);
        r += ".";
    }

    r += node.name;
}

void DescVisitor::visit(NamespaceNode &node)
{
    r += "namespace ";
    node.name->accept(*this);
}

void DescVisitor::visit(TypeNode &node)
{
    node.name->accept(*this);
}

void DescVisitor::visit(VarNode &node)
{
    node.name->accept(*this);

    if(node.type)
    {
        r += ":";
        node.type->accept(*this);
    }
}
