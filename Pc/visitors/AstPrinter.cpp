#include "AstPrinter.h"

#include "application/Context.h"

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
#include "nodes/ReturnNode.h"
#include "nodes/InitNode.h"
#include "nodes/IncDecNodes.h"
#include "nodes/WhileNode.h"
#include "nodes/IfNode.h"
#include "nodes/ForNode.h"
#include "nodes/CommaNode.h"
#include "nodes/InlineVarNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include "visitors/NameVisitors.h"

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
        r += pcx::str(" -> ", sym->fullname());

        if(auto t = sym->findProperty("type"))
        {
            auto type = t.to<Type*>();
            r += pcx::str(" -> ", type->text());
        }
    }

    if(auto t = node.findProperty("type"))
    {
        auto type = t.to<Type*>();
        r += pcx::str(" -> ", type->text());
    }

    if(auto o = node.findProperty("offset"))
    {
        r += pcx::str(" #", o.to<std::size_t>());
    }

    return r;
}

}

AstPrinter::AstPrinter(Context &c, std::ostream &os) : c(c), os(os), tc(0)
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
    if(Visitor::query<NameVisitors::LastIdOfName, std::string>(node.name.get()) != "std" || !c.option("debug", "suppress_std"))
    {
        tab() << "namespace " << node.description() << details(node) << "\n";
        node.body->accept(*this);
    }
}

void AstPrinter::visit(TypeNode &node)
{
    tab() << "type " << node.description() << details(node) << "\n";
}

void AstPrinter::visit(FuncNode &node)
{
    if(!node.autoGen || !c.option("debug", "suppress_autogens"))
    {
        tab() << (node.findProperty("free").value<bool>() ? "free " : "") << "func " << node.description() << details(node) << "\n";

        if(!node.inits.empty())
        {
            tab() << "inits\n";

            auto g = pcx::scoped_counter(tc);
            for(auto &i: node.inits)
            {
                i->accept(*this);
            }
        }

        if(node.body)
        {
            node.body->accept(*this);
        }
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
    tab() << (node.findProperty("free").value<bool>() ? "free " : "") << "var " << node.description() << details(node) << "\n";

    if(node.value)
    {
        auto g = pcx::scoped_counter(tc);
        node.value->accept(*this);
    }
}

void AstPrinter::visit(CharLiteralNode &node)
{
    tab() << "char literal " << node.description() << details(node) << "\n";
}

void AstPrinter::visit(IntLiteralNode &node)
{
    tab() << "int literal " << node.description() << details(node) << "\n";
}

void AstPrinter::visit(BoolLiteralNode &node)
{
    tab() << "bool literal " << node.description() << details(node) << "\n";
}

void AstPrinter::visit(StringLiteralNode &node)
{
    tab() << "string literal " << node.description() << details(node) << "\n";
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

    if(node.target)
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

void AstPrinter::visit(ThisNode &node)
{
    tab() << "this" << details(node) << "\n";
}

void AstPrinter::visit(AssignNode &node)
{
    tab() << "assign" << details(node) << "\n";

    if(true)
    {
        auto g1 = pcx::scoped_counter(tc);
        tab() << "target\n";

        auto g2 = pcx::scoped_counter(tc);
        node.target->accept(*this);
    }

    if(true)
    {
        auto g1 = pcx::scoped_counter(tc);
        tab() << "expr\n";

        auto g2 = pcx::scoped_counter(tc);
        node.expr->accept(*this);
    }
}

void AstPrinter::visit(UnaryNode &node)
{
    tab() << "unary" << node.token.text() << details(node) << "\n";

    auto g = pcx::scoped_counter(tc);
    node.expr->accept(*this);
}

void AstPrinter::visit(BinaryNode &node)
{
    tab() << "binary" << node.token.text() << details(node) << "\n";

    auto g = pcx::scoped_counter(tc);
    node.left->accept(*this);
    node.right->accept(*this);
}

void AstPrinter::visit(LogicalNode &node)
{
    tab() << "logical" << node.token.text() << details(node) << "\n";

    auto g = pcx::scoped_counter(tc);
    node.left->accept(*this);
    node.right->accept(*this);
}

void AstPrinter::visit(ReturnNode &node)
{
    tab() << "return\n";

    if(node.expr)
    {
        auto g = pcx::scoped_counter(tc);
        node.expr->accept(*this);
    }
}

void AstPrinter::visit(InitNode &node)
{
    tab() << "init " << node.name << details(node) << "\n";

    if(node.target)
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

void AstPrinter::visit(PreIncDecNode &node)
{
    tab() << "pre" << node.token.text() << "\n";

    auto g = pcx::scoped_counter(tc);
    node.expr->accept(*this);
}

void AstPrinter::visit(PostIncDecNode &node)
{
    tab() << "post" << node.token.text() << "\n";

    auto g = pcx::scoped_counter(tc);
    node.expr->accept(*this);
}

void AstPrinter::visit(WhileNode &node)
{
    tab() << "while\n";

    auto g = pcx::scoped_counter(tc);
    node.expr->accept(*this);
    node.body->accept(*this);
}

void AstPrinter::visit(IfNode &node)
{
    tab() << "if\n";

    auto g = pcx::scoped_counter(tc);
    node.expr->accept(*this);
    node.body->accept(*this);

    if(node.elseBody)
    {
        node.elseBody->accept(*this);
    }
}

void AstPrinter::visit(ForNode &node)
{
    tab() << "for\n";

    auto g = pcx::scoped_counter(tc);

    if(node.init)
    {
        node.init->accept(*this);
    }

    if(node.cond)
    {
        node.cond->accept(*this);
    }

    if(node.post)
    {
        node.post->accept(*this);
    }

    node.body->accept(*this);
}

void AstPrinter::visit(CommaNode &node)
{
    tab() << "comma\n";

    auto g = pcx::scoped_counter(tc);
    node.first->accept(*this);
    node.second->accept(*this);
}

void AstPrinter::visit(InlineVarNode &node)
{
    tab() << "inlinevar\n";
    node.body->accept(*this);
}

std::ostream &AstPrinter::tab() const
{
    return os << std::string(tc * 4, ' ');
}
