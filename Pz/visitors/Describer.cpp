#include "Describer.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/TypeNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"
#include "nodes/FuncNode.h"
#include "nodes/LiteralNodes.h"

#include <pcx/join_str.h>
#include <pcx/lexical_cast.h>

namespace
{

std::string describe(const NodePtr &n){ return n->description(); }

}

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

void Describer::visit(TypeNode &node)
{
    if(node.constant)
    {
        r += "const ";
    }

    if(node.ref)
    {
        r += "ref ";
    }

    for(std::size_t i = 0; i < node.ptr; ++i)
    {
        r += "ptr ";
    }

    if(node.name)
    {
        node.name->accept(*this);
    }

    if(!node.args.empty())
    {
        r += "(" + pcx::join_str(node.args, ",", describe) + ")";
    }

    if(node.returnType)
    {
        r += ":";
        node.returnType->accept(*this);
    }
}

void Describer::visit(ClassNode &node)
{
    node.name->accept(*this);
}

void Describer::visit(VarNode &node)
{
    node.name->accept(*this);

    if(node.type)
    {
        r += ":";
        node.type->accept(*this);
    }
}

void Describer::visit(FuncNode &node)
{
    node.name->accept(*this);

    r += "(" + pcx::join_str(node.args, ", ", describe) + ")";

    if(node.type)
    {
        r += ":";
        node.type->accept(*this);
    }
}

void Describer::visit(ScopeNode &node)
{
    r += "{ scope }";
}

void Describer::visit(IntLiteralNode &node)
{
    r += pcx::lexical_cast<std::string>(node.value);
}
