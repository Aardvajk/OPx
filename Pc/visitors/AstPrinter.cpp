#include "AstPrinter.h"

#include "scanner/Lexer.h"

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
#include "nodes/ThisNode.h"
#include "nodes/DerefNode.h"
#include "nodes/UnaryNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/SubscriptNode.h"
#include "nodes/WhileNode.h"
#include "nodes/PrimitiveCastNode.h"
#include "nodes/LogicalNode.h"
#include "nodes/IfNode.h"
#include "nodes/InitNode.h"
#include "nodes/ForNode.h"
#include "nodes/PragmaNode.h"
#include "nodes/IncDecNode.h"
#include "nodes/OpEqNode.h"

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
            n->accept(*this);
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
        os << " -> " << s->fullname();

        if(auto tp = s->getProperty("type"))
        {
            os << ":" << tp.to<const Type*>()->text() << " [" << s << "]";
        }
    }

    if(auto s = node.getProperty("matches"))
    {
        auto sv = s.to<std::vector<Sym*> >();
        for(auto s: sv)
        {
            os << " {" << s->fullname() << "}";
        }
    }

    os << "\n";

    if(node.parent)
    {
        auto g = pcx::scoped_counter(tc);
        node.parent->accept(*this);
    }
}

void AstPrinter::visit(NamespaceNode &node)
{
    tab() << "namespace " << NameVisitors::prettyName(node.name.get()) << "\n";
    node.body->accept(*this);
}

void AstPrinter::visit(ClassNode &node)
{
    tab() << "class " << NameVisitors::prettyName(node.name.get());

    if(auto s = node.getProperty("sym"))
    {
        os << " -> " << s.to<const Sym*>()->fullname();
    }

    os << "\n";

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

    if(!node.params.empty())
    {
        std::vector<std::string> av;
        for(auto &a: node.params)
        {
            av.push_back(NameVisitors::prettyName(a.get()));
        }

        os << "(" << pcx::join_str(av, ", ") << ")";
    }

    if(auto s = node.getProperty("sym"))
    {
        auto sym = s.to<const Sym*>();
        os << " -> " << sym->fullname() << ":" << sym->property<const Type*>("type")->text();
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

    std::vector<std::string> av;
    for(auto &a: node.args)
    {
        av.push_back(NameVisitors::prettyName(a.get()));
    }

    os << "(" << pcx::join_str(av, ", ") << ")";

    if(node.type)
    {
        os << ":" << NameVisitors::prettyName(node.type.get());
    }

    if(node.getProperty("sym"))
    {
        os << " -> " << node.property<const Sym*>("sym")->fullname();
    }

    os << "\n";

    for(auto &i: node.inits)
    {
        i->accept(*this);
    }

    if(node.initialisers)
    {
        node.initialisers->accept(*this);
    }

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
    tab() << "type " << (node.constant ? "const " : "") << (node.ref ? "ref " : "") << " ptr(" << node.ptr << ")\n";

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
            a->accept(*this);
        }
    }
    else
    {
        auto g = pcx::scoped_counter(tc);
        node.name->accept(*this);
    }

    if(node.sub)
    {
        auto g = pcx::scoped_counter(tc);
        node.sub->accept(*this);
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

void AstPrinter::visit(BoolLiteralNode &node)
{
    tab() << "bool literal " << (node.value ? "true" : "false") << "\n";
}

void AstPrinter::visit(SizeLiteralNode &node)
{
    tab() << "size literal " << node.value << "\n";
}

void AstPrinter::visit(StringLiteralNode &node)
{
    tab() << "string literal \"" << Lexer::encodeString(node.value) << "\"\n";
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
            p->accept(*this);
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

void AstPrinter::visit(ThisNode &node)
{
    tab() << "this";

    if(auto sp = node.getProperty("sym"))
    {
        auto s = sp.to<const Sym*>();
        os << " -> " << s->fullname() << ":" << s->property<const Type*>("type")->text() << " [" << s << "]";
    }

    os << "\n";
}

void AstPrinter::visit(DerefNode &node)
{
    tab() << "deref\n";

    auto g = pcx::scoped_counter(tc);
    node.expr->accept(*this);
}

void AstPrinter::visit(UnaryNode &node)
{
    tab() << "unary " << Operators::toString(node.op) << "\n";

    auto g = pcx::scoped_counter(tc);
    node.expr->accept(*this);
}

void AstPrinter::visit(BinaryNode &node)
{
    tab() << "binary " << Operators::toString(node.op) << "\n";

    auto g = pcx::scoped_counter(tc);
    node.left->accept(*this);
    node.right->accept(*this);
}

void AstPrinter::visit(SubscriptNode &node)
{
    tab() << "subscript\n";

    auto g = pcx::scoped_counter(tc);
    node.target->accept(*this);
    node.expr->accept(*this);
}

void AstPrinter::visit(WhileNode &node)
{
    tab() << "while\n";

    auto g = pcx::scoped_counter(tc);
    node.expr->accept(*this);
    node.body->accept(*this);
}

void AstPrinter::visit(PrimitiveCastNode &node)
{
    tab() << "primitive cast to " << node.type->text() << "\n";

    if(node.expr)
    {
        auto g = pcx::scoped_counter(tc);
        node.expr->accept(*this);
    }
}

void AstPrinter::visit(LogicalNode &node)
{
    tab() << "logical " << Operators::toString(node.op) << "\n";

    auto g = pcx::scoped_counter(tc);
    node.left->accept(*this);
    node.right->accept(*this);
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

void AstPrinter::visit(InitNode &node)
{
    tab() << "init " << node.name;

    if(auto sp = node.getProperty("sym"))
    {
        auto s = sp.to<const Sym*>();
        os << " -> " << s->fullname() << ":" << s->property<const Type*>("type")->text() << " [" << s << "]";
    }

    os << "\n";

    auto g = pcx::scoped_counter(tc);
    for(auto &p: node.params)
    {
        p->accept(*this);
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

void AstPrinter::visit(PragmaNode &node)
{
    tab() << "pragma " << Pragmas::toString(node.cmd);

    if(!node.arg.empty())
    {
        os << " \"" << Lexer::encodeString(node.arg) << "\"";
    }

    os << "\n";
}

void AstPrinter::visit(IncDecNode &node)
{
    tab() << "incdec " << Operators::toString(node.op) << "\n";

    auto g = pcx::scoped_counter(tc);
    node.target->accept(*this);
}

void AstPrinter::visit(OpEqNode &node)
{
    tab() << "opeq " << Operators::toString(node.op) << "\n";

    auto g = pcx::scoped_counter(tc);
    node.target->accept(*this);
    node.expr->accept(*this);
}

std::ostream &AstPrinter::tab() const
{
    return os << std::string(tc * 4, ' ');
}
