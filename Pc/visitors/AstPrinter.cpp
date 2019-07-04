#include "AstPrinter.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/ClassNode.h"
#include "nodes/GlobalNode.h"
#include "nodes/IdNode.h"
#include "nodes/DotNode.h"
#include "nodes/VarNode.h"
#include "nodes/FuncDecNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/CharLiteralNode.h"
#include "nodes/IntLiteralNode.h"
#include "nodes/ExprNode.h"
#include "nodes/CallNode.h"
#include "nodes/ReturnNode.h"

#include "symbols/Sym.h"

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

void AstPrinter::visit(NamespaceNode &node)
{
    tab() << "namespace " << node.sym->fullname() << "\n";
    node.block->accept(*this);
}

void AstPrinter::visit(ClassNode &node)
{
    tab() << "class " << node.sym->fullname() << "\n";
    if(node.block)
    {
        node.block->accept(*this);
    }
}

#include "symbols/SymResult.h"

void print(Node &node)
{
    for(auto s: node.property("syms").value<std::vector<SymResult> >())
    {
        std::cout << "---> " << s.sym->fullname() << "\n";
    }
}

void AstPrinter::visit(GlobalNode &node)
{
    tab() << "global\n";

    auto g = pcx::scoped_counter(tc);
    node.child->accept(*this);
}

void AstPrinter::visit(IdNode &node)
{
    tab() << "id " << node.name << "\n";
    print(node);
}

void AstPrinter::visit(DotNode &node)
{
    tab() << "dot " << node.name << "\n";
    print(node);

    auto g = pcx::scoped_counter(tc);
    node.child->accept(*this);
}

void AstPrinter::visit(VarNode &node)
{
    tab() << "var " << node.sym->fullname() << ":" << node.sym->property("type").to<const Type*>()->text() << "\n";
    if(node.value)
    {
        auto g = pcx::scoped_counter(tc);
        node.value->accept(*this);
    }
}

void AstPrinter::visit(FuncDecNode &node)
{
    tab() << "funcdec " << node.sym->fullname() << ":" << node.sym->property("type").to<const Type*>()->text() << "\n";
}

void AstPrinter::visit(FuncNode &node)
{
    tab() << "func " << node.sym->fullname() << ":" << node.sym->property("type").to<const Type*>()->text() << "\n";
    node.block->accept(*this);
}

void AstPrinter::visit(ScopeNode &node)
{
    tab() << "scope\n";
    if(node.block)
    {
        node.block->accept(*this);
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
    node.node->accept(*this);
}

void AstPrinter::visit(CallNode &node)
{
    tab() << "call\n";

    auto g = pcx::scoped_counter(tc);
    node.target->accept(*this);

    for(auto &a: node.args)
    {
        auto g = pcx::scoped_counter(tc);
        a.accept(*this);
    }
}

void AstPrinter::visit(ReturnNode &node)
{
    tab() << "return\n";

    auto g = pcx::scoped_counter(tc);
    node.expr->accept(*this);
}

std::ostream &AstPrinter::tab() const
{
    return os << std::string(tc * 4, ' ');
}
