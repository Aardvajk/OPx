#include "NameVisitors.h"

#include "nodes/IdNode.h"
#include "nodes/DotNode.h"

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

std::string NameVisitors::prettyName(Node *node)
{
    PrettyName pn;
    node->accept(pn);

    return pn.result();
}
