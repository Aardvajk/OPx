#include "AstPrinter.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/DotNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"
#include "nodes/FuncNode.h"
#include "nodes/TypeNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/ExprNode.h"
#include "nodes/CallNode.h"

#include "visitors/NameVisitors.h"

#include <pcx/scoped_counter.h>

#include <iostream>

AstPrinter::AstPrinter(std::ostream &os) : os(os), tc(0)
{
}

void AstPrinter::visit(BlockNode &node)
{
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
    tab() << "id " << node.name << "\n";
}

void AstPrinter::visit(DotNode &node)
{
    tab() << "dot " << node.name << "\n";

    auto g = pcx::scoped_counter(tc);
    node.child->accept(*this);
}

void AstPrinter::visit(NamespaceNode &node)
{
    tab() << "namespace " << NameVisitors::prettyName(node.name.get()) << "\n";
    node.block->accept(*this);
}

void AstPrinter::visit(ClassNode &node)
{
    tab() << "class " << NameVisitors::prettyName(node.name.get()) << "\n";

    if(node.block)
    {
        node.block->accept(*this);
    }
}

void AstPrinter::visit(VarNode &node)
{
    tab() << "var " << NameVisitors::prettyName(node.name.get()) << "\n";

    if(node.type)
    {
        auto g = pcx::scoped_counter(tc);
        node.type->accept(*this);
    }
}

void AstPrinter::visit(FuncNode &node)
{
    tab() << "func " << NameVisitors::prettyName(node.name.get()) << "\n";

    if(!node.args.empty())
    {
        auto g = pcx::scoped_counter(tc);
        tab() << "args\n";

        for(auto &a: node.args)
        {
            a.accept(*this);
        }
    }

    if(node.type)
    {
        auto g = pcx::scoped_counter(tc);
        node.type->accept(*this);
    }

    if(node.block)
    {
        auto g = pcx::scoped_counter(tc);

        tab() << "body\n";
        node.block->accept(*this);
    }
}

void AstPrinter::visit(TypeNode &node)
{
    tab() << "type\n";

    if(node.name)
    {
        auto g = pcx::scoped_counter(tc);
        node.name->accept(*this);
    }
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

std::ostream &AstPrinter::tab() const
{
    return os << std::string(tc * 4, ' ');
}
