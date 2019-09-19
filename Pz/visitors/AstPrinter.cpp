#include "AstPrinter.h"

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

#include "syms/Sym.h"

#include "types/Type.h"

#include <pcx/str.h>
#include <pcx/join_str.h>
#include <pcx/scoped_counter.h>

namespace
{

std::string details(Node &node)
{
    std::string r;

    if(auto s = node.findProperty("sym"))
    {
        auto sym = s.to<Sym*>();
        r += pcx::str(" -> ", sym->fullname(), " [", sym, "]");
    }

    if(auto s = node.findProperty("syms"))
    {
        auto syms = s.to<std::vector<Sym*> >();
        r += pcx::str(" -> ", pcx::join_str(syms, ", ", [](const Sym *s){ return s->fullname(); }));
    }

    if(auto t = node.findProperty("type"))
    {
        auto type = t.to<Type*>();
        r += pcx::str(" -> ", type->text());
    }

    return r;
}

}

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
            n->accept(*this);
        }
    }

    tab() << "}\n";
}

void AstPrinter::visit(IdNode &node)
{
    tab() << "id " << node.name << details(node) << "\n";

    if(node.parent)
    {
        auto g = pcx::scoped_counter(tc);
        node.parent->accept(*this);
    }
}

void AstPrinter::visit(NamespaceNode &node)
{
    tab() << "namespace " << node.description() << details(node) << "\n";
    node.body->accept(*this);
}

void AstPrinter::visit(TypeNode &node)
{
    tab() << "type " << node.description() << details(node) << "\n";
}

void AstPrinter::visit(FuncNode &node)
{
    tab() << "func " << node.description() << details(node) << "\n";

    if(node.body)
    {
        node.body->accept(*this);
    }
}

void AstPrinter::visit(ScopeNode &node)
{
    tab() << "scope" << details(node) << "\n";
    node.body->accept(*this);
}

void AstPrinter::visit(ClassNode &node)
{
    tab() << "class " << node.description() << details(node) << "\n";

    if(node.body)
    {
        node.body->accept(*this);
    }
}

void AstPrinter::visit(VarNode &node)
{
    tab() << "var " << node.description() << details(node) << "\n";

    if(node.value)
    {
        auto g = pcx::scoped_counter(tc);
        node.value->accept(*this);
    }
}

void AstPrinter::visit(IntLiteralNode &node)
{
    tab() << "int literal " << node.value << details(node) << "\n";
}

void AstPrinter::visit(ExprNode &node)
{
    tab() << "expr\n";

    auto g = pcx::scoped_counter(tc);
    node.expr->accept(*this);
}

void AstPrinter::visit(CallNode &node)
{
    tab() << "call" << details(node) << "\n";

    if(true)
    {
        auto g1 = pcx::scoped_counter(tc);
        tab() << "target\n";

        auto g2 = pcx::scoped_counter(tc);
        node.target->accept(*this);
    }

    if(!node.params.empty())
    {
        auto g1 = pcx::scoped_counter(tc);
        tab() << "params\n";

        auto g2 = pcx::scoped_counter(tc);
        for(auto &p: node.params)
        {
            p->accept(*this);
        }
    }
}

void AstPrinter::visit(ConstructNode &node)
{
    tab() << "construct " << node.type->text() << details(node) << "\n";

    for(auto &p: node.params)
    {
        p->accept(*this);
    }
}

void AstPrinter::visit(PragmaNode &node)
{
    tab() << node.description() << "\n";
}

void AstPrinter::visit(AddrOfNode &node)
{
    tab() << "addrof" << details(node) << "\n";

    auto g = pcx::scoped_counter(tc);
    node.expr->accept(*this);
}

void AstPrinter::visit(DerefNode &node)
{
    tab() << "deref" << details(node) << "\n";

    auto g = pcx::scoped_counter(tc);
    node.expr->accept(*this);
}

std::ostream &AstPrinter::tab() const
{
    return os << std::string(tc * 4, ' ');
}
