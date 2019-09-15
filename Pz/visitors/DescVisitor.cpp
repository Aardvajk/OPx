#include "DescVisitor.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/TypeNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"

#include <pcx/join_str.h>

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
    node.name->accept(*this);
}

void DescVisitor::visit(TypeNode &node)
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

    node.name->accept(*this);
}

void DescVisitor::visit(FuncNode &node)
{
    node.name->accept(*this);

    r += "(";
    r += pcx::join_str(node.args, ", ", [](const NodePtr &n){ return n->description(); });
    r += ")";

    if(node.type)
    {
        r += ":";
        node.type->accept(*this);
    }
}

void DescVisitor::visit(ScopeNode &node)
{
    r += "(scope)";
}

void DescVisitor::visit(ClassNode &node)
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
