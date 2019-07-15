#include "NameVisitors.h"

#include "nodes/IdNode.h"
#include "nodes/VarNode.h"
#include "nodes/TypeNode.h"
#include "nodes/LiteralNodes.h"

#include <pcx/str.h>
#include <pcx/join_str.h>

NameVisitors::PrettyName::PrettyName()
{
}

void NameVisitors::PrettyName::visit(IdNode &node)
{
    r += node.name;
    if(node.child)
    {
        r += ".";
        node.child->accept(*this);
    }
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
    for(std::size_t i = 0; i < node.ptr; ++i)
    {
        r += "ptr ";
    }

    if(node.function)
    {
        std::vector<std::string> v;
        for(auto &a: node.args)
        {
            PrettyName p;
            a.accept(p);

            v.push_back(p.result());
        }

        r += pcx::str("(", pcx::join_str(v, ", "), ")");

        if(node.returnType)
        {
            r += ":";
            node.returnType->accept(*this);
        }
    }
    else
    {
        if(node.name)
        {
            node.name->accept(*this);
        }
    }
}

void NameVisitors::PrettyName::visit(IntLiteralNode &node)
{
    r += pcx::str(node.value);
}

std::string NameVisitors::prettyName(Node *node)
{
    PrettyName pn;
    node->accept(pn);

    return pn.result();
}

NameVisitors::IsNameSimple::IsNameSimple() : r(false)
{
}

void NameVisitors::IsNameSimple::visit(IdNode &node)
{
    if(!node.child)
    {
        r = true;
    }
}

bool NameVisitors::isNameSimple(Node *node)
{
    IsNameSimple nv;
    node->accept(nv);

    return nv.result();
}

NameVisitors::LastIdOfName::LastIdOfName()
{
}

void NameVisitors::LastIdOfName::visit(IdNode &node)
{
    if(node.child)
    {
        node.child->accept(*this);
    }
    else
    {
        r = node.name;
    }
}

std::string NameVisitors::lastIdOfName(Node *node)
{
    LastIdOfName in;
    node->accept(in);

    return in.result();
}

