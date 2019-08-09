#include "NameVisitors.h"

#include "scanner/Lexer.h"

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
    if(node.parent)
    {
        node.parent->accept(*this);
        r += ".";
    }

    r += node.name;
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
            a->accept(p);

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

    if(node.sub)
    {
        r += "[";
        node.sub->accept(*this);
        r += "]";
    }
}

void NameVisitors::PrettyName::visit(NullLiteralNode &node)
{
    r += "null";
}

void NameVisitors::PrettyName::visit(IntLiteralNode &node)
{
    r += pcx::str(node.value);
}

void NameVisitors::PrettyName::visit(CharLiteralNode &node)
{
    r += pcx::str("\'", Lexer::encodeString(std::string(1, node.value)), "\'");
}

void NameVisitors::PrettyName::visit(BoolLiteralNode &node)
{
    r += node.value ? "true" : "false";
}

void NameVisitors::PrettyName::visit(SizeLiteralNode &node)
{
    r += pcx::str(node.value);
}

void NameVisitors::PrettyName::visit(ThisNode &node)
{
    r += "this";
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
    if(!node.parent)
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
    r = node.name;
}

std::string NameVisitors::lastIdOfName(Node *node)
{
    LastIdOfName in;
    node->accept(in);

    return in.result();
}

NameVisitors::IsNameSpecial::IsNameSpecial() : r(Token::Type::Invalid)
{
}

void NameVisitors::IsNameSpecial::visit(IdNode &node)
{
    r = node.special;
}

Token::Type NameVisitors::isNameSpecial(Node *node)
{
    IsNameSpecial iv;
    node->accept(iv);

    return iv.result();
}
