#include "NameVisitors.h"

#include "nodes/IdNode.h"
#include "nodes/DotNode.h"
#include "nodes/VarNode.h"
#include "nodes/TypeNode.h"

NameVisitors::PrettyName::PrettyName()
{
}

void NameVisitors::PrettyName::visit(IdNode &node)
{
    r += node.name;
}

void NameVisitors::PrettyName::visit(DotNode &node)
{
    r += node.name + ".";
    node.child->accept(*this);
}

void NameVisitors::PrettyName::visit(VarNode &node)
{
    node.name->accept(*this);

    if(node.type)
    {
        r += ":";
        node.type->accept(*this);
    }
}

void NameVisitors::PrettyName::visit(TypeNode &node)
{
    if(node.name)
    {
        node.name->accept(*this);
    }
}

std::string NameVisitors::prettyName(Node *node)
{
    PrettyName pn;
    node->accept(pn);

    return pn.result();
}
