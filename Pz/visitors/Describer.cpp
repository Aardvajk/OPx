#include "Describer.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/FuncNode.h"

Describer::Describer()
{
}

void Describer::visit(BlockNode &node)
{
    r = "{ block }";
}

void Describer::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent->accept(*this);
        r += ".";
    }

    r += node.name;
}

void Describer::visit(NamespaceNode &node)
{
    node.name->accept(*this);
}

void Describer::visit(FuncNode &node)
{
    node.name->accept(*this);
    r += "()";
}
