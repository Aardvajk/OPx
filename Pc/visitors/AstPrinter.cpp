#include "AstPrinter.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/TypeNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/ExprNode.h"
#include "nodes/CallNode.h"
#include "nodes/ReturnNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/AssignNode.h"

#include "visitors/NameVisitors.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include <pcx/scoped_counter.h>
#include <pcx/join_str.h>

#include <iostream>

AstPrinter::AstPrinter(std::ostream &os) : os(os), tc(0)
{
}

void AstPrinter::visit(BlockNode &node)
{
    tab() << "block\n";
    tab() << "{\n";

    if(true)
    {
        auto g = pcx::scoped_counter(tc);
        for(auto &n: node.nodes)
        {
            n.accept(*this);
        }
    }

    tab() << "}\n";
}

void AstPrinter::visit(IdNode &node)
{
    tab() << "id " << node.name;

    if(auto sp = node.getProperty("sym"))
    {
        auto s = sp.to<const Sym*>();
        os << " -> " << s->fullname() << ":" << s->property<const Type*>("type")->text() << " [" << s << "]";
    }

    os << "\n";

    if(node.child)
    {
        auto g = pcx::scoped_counter(tc);
        node.child->accept(*this);
    }
}

void AstPrinter::visit(NamespaceNode &node)
{
    tab() << "namespace " << NameVisitors::prettyName(node.name.get()) << "\n";
    node.body->accept(*this);
}

void AstPrinter::visit(ClassNode &node)
{
    tab() << "class " << NameVisitors::prettyName(node.name.get()) << "\n";

    if(node.body)
    {
        node.body->accept(*this);
    }
}

void AstPrinter::visit(VarNode &node)
{
    tab() << "var " << NameVisitors::prettyName(node.name.get());

    if(node.type)
    {
        os << ":" << NameVisitors::prettyName(node.type.get());
    }

    os << "\n";

    if(node.value)
    {
        auto g = pcx::scoped_counter(tc);
        node.value->accept(*this);
    }
}

void AstPrinter::visit(FuncNode &node)
{
    tab() << "func " << NameVisitors::prettyName(node.name.get());

    os << "(";

    std::vector<std::string> av;
    for(auto &a: node.args)
    {
        av.push_back(NameVisitors::prettyName(&a));
    }

    os << pcx::join_str(av, ", ") << ")";

    if(node.type)
    {
        os << ":" << NameVisitors::prettyName(node.type.get());
    }

    os << "\n";

    if(node.body)
    {
        node.body->accept(*this);
    }
}

void AstPrinter::visit(ScopeNode &node)
{
    tab() << "scope\n";
    node.body->accept(*this);
}

void AstPrinter::visit(TypeNode &node)
{
    tab() << "type ptr(" << node.ptr << ")\n";

    if(node.function)
    {
        auto g = pcx::scoped_counter(tc);
        if(node.returnType)
        {
            node.name->accept(*this);
        }

        for(auto &a: node.args)
        {
            auto g = pcx::scoped_counter(tc);
            a.accept(*this);
        }
    }
    else
    {
        auto g = pcx::scoped_counter(tc);
        node.name->accept(*this);
    }
}

void AstPrinter::visit(NullLiteralNode &node)
{
    tab() << "null literal\n";
}

void AstPrinter::visit(CharLiteralNode &node)
{
    tab() << "char literal " << node.value << "\n";
}

void AstPrinter::visit(IntLiteralNode &node)
{
    tab() << "int literal " << node.value << "\n";
}

void AstPrinter::visit(ExprNode &node)
{
    tab() << "expr\n";

    auto g = pcx::scoped_counter(tc);
    node.expr->accept(*this);
}

void AstPrinter::visit(CallNode &node)
{
    tab() << "call\n";

    auto g = pcx::scoped_counter(tc);
    node.target->accept(*this);

    if(true)
    {
        tab() << "params\n";

        auto g = pcx::scoped_counter(tc);
        for(auto &p: node.params)
        {
            p.accept(*this);
        }
    }
}

void AstPrinter::visit(ReturnNode &node)
{
    tab() << "return\n";

    auto g = pcx::scoped_counter(tc);
    node.expr->accept(*this);
}

void AstPrinter::visit(AddrOfNode &node)
{
    tab() << "addrof\n";

    auto g = pcx::scoped_counter(tc);
    node.expr->accept(*this);
}

void AstPrinter::visit(AssignNode &node)
{
    tab() << "assign\n";

    auto g = pcx::scoped_counter(tc);
    node.target->accept(*this);
    node.expr->accept(*this);
}

std::ostream &AstPrinter::tab() const
{
    return os << std::string(tc * 4, ' ');
}
