#include "NameVisitors.h"

#include "nodes/GlobalNode.h"
#include "nodes/IdNode.h"
#include "nodes/DotNode.h"

NameVisitors::IsNameSimple::IsNameSimple() : r(false)
{
}

void NameVisitors::IsNameSimple::visit(IdNode &node)
{
    r = true;
}

bool NameVisitors::isNameSimple(Node *node)
{
    IsNameSimple is;
    node->accept(is);

    return is.result();
}

NameVisitors::LastIdOfName::LastIdOfName()
{
}

void NameVisitors::LastIdOfName::visit(GlobalNode &node)
{
    node.child->accept(*this);
}

void NameVisitors::LastIdOfName::visit(IdNode &node)
{
    r = node.name;
}

void NameVisitors::LastIdOfName::visit(DotNode &node)
{
    node.child->accept(*this);
}

std::string NameVisitors::lastIdOfName(Node *node)
{
    LastIdOfName in;
    node->accept(in);

    return in.result();
}

NameVisitors::PrettyName::PrettyName()
{
}

void NameVisitors::PrettyName::visit(GlobalNode &node)
{
    r += ".";
    node.child->accept(*this);
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
