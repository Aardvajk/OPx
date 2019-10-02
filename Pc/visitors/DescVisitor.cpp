#include "DescVisitor.h"

#include "scanner/Lexer.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/TypeNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/ExprNode.h"
#include "nodes/CallNode.h"
#include "nodes/ConstructNode.h"
#include "nodes/PragmaNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/DerefNode.h"
#include "nodes/ThisNode.h"
#include "nodes/AssignNode.h"
#include "nodes/UnaryNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/LogicalNode.h"
#include "nodes/InitNode.h"
#include "nodes/IncDecNodes.h"
#include "nodes/TextNode.h"
#include "nodes/CommaNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

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
        r += node.arrow ? "->" : ".";
    }

    r += node.name;
}

void DescVisitor::visit(NamespaceNode &node)
{
    node.name->accept(*this);
}

void DescVisitor::visit(TypeNode &node)
{
    if(auto t = node.findProperty("type"))
    {
        r = t.to<Type*>()->text();
    }
    else
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
            r += "(" + pcx::join_str(node.args, ", ", [](const NodePtr &n){ return n->description(); }) + ")";
        }

        if(node.returnType)
        {
            r += ":";
            node.returnType->accept(*this);
        }
    }
}

void DescVisitor::visit(FuncNode &node)
{
    if(node.findProperty("free").value<bool>())
    {
        r += "free ";
    }

    node.name->accept(*this);

    r += "(";
    r += pcx::join_str(node.args, ", ", [](const NodePtr &n){ return n->description(); });
    r += ")";

    if(node.type)
    {
        r += ":";
        node.type->accept(*this);
    }

    if(node.constMethod)
    {
        r += " const";
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
    if(node.findProperty("free").value<bool>())
    {
        r += "free ";
    }

    node.name->accept(*this);

    if(auto s = node.findProperty("sym"))
    {
        r += ":" + s.to<Sym*>()->property<Type*>("type")->text();
    }
    else if(node.type)
    {
        r += ":";
        node.type->accept(*this);
    }
}

void DescVisitor::visit(ExprNode &node)
{
    node.expr->accept(*this);
}

void DescVisitor::visit(CharLiteralNode &node)
{
    r += pcx::str('\'', Lexer::encodeString(std::string(1, node.value)), '\'');
}

void DescVisitor::visit(IntLiteralNode &node)
{
    r += pcx::str(node.value);
}

void DescVisitor::visit(BoolLiteralNode &node)
{
    r += node.value ? "true" : "false";
}

void DescVisitor::visit(StringLiteralNode &node)
{
    r += pcx::str("\"", Lexer::encodeString(node.value), "\"");
}

void DescVisitor::visit(CallNode &node)
{
    node.target->accept(*this);

    r += "(";
    r += pcx::join_str(node.params, ", ", [](const NodePtr &n){ return n->description(); });
    r += ")";
}

void DescVisitor::visit(ConstructNode &node)
{
    r += node.type->text();

    r += "(";
    r += pcx::join_str(node.params, ", ", [](const NodePtr &n){ return n->description(); });
    r += ")";
}

void DescVisitor::visit(PragmaNode &node)
{
    r += "pragma(";
    r += Pragmas::toString(node.type);

    if(!node.arg.empty())
    {
        r += pcx::str(", \"", Lexer::encodeString(node.arg), "\"");
    }

    r += ")";
}

void DescVisitor::visit(AddrOfNode &node)
{
    r += "&";
    node.expr->accept(*this);
}

void DescVisitor::visit(DerefNode &node)
{
    r += "*";
    node.expr->accept(*this);
}

void DescVisitor::visit(ThisNode &node)
{
    r += "this";
}

void DescVisitor::visit(AssignNode &node)
{
    node.target->accept(*this);
    r += " = ";
    node.expr->accept(*this);
}

void DescVisitor::visit(UnaryNode &node)
{
    r += pcx::str(node.token.text());
    node.expr->accept(*this);
}

void DescVisitor::visit(BinaryNode &node)
{
    node.left->accept(*this);
    r += pcx::str(' ', node.token.text(), ' ');
    node.right->accept(*this);
}

void DescVisitor::visit(LogicalNode &node)
{
    node.left->accept(*this);
    r += pcx::str(' ', node.token.text(), ' ');
    node.right->accept(*this);
}

void DescVisitor::visit(InitNode &node)
{
    r += pcx::str(node.name, "(");
    r += pcx::join_str(node.params, ", ", [](const NodePtr &n){ return n->description(); });
    r += ")";
}

void DescVisitor::visit(PreIncDecNode &node)
{
    r += node.token.text();
    node.expr->accept(*this);
}

void DescVisitor::visit(PostIncDecNode &node)
{
    node.expr->accept(*this);
    r += node.token.text();
}

void DescVisitor::visit(TextNode &node)
{
    r += node.value;
}

void DescVisitor::visit(CommaNode &node)
{
    node.first->accept(*this);
    r += ", ";
    node.second->accept(*this);
}
