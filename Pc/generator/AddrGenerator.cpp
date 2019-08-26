#include "AddrGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/CallNode.h"
#include "nodes/DerefNode.h"

#include "visitors/NameVisitors.h"
#include "visitors/TypeVisitor.h"

#include "types/Type.h"

#include "generator/ExprGenerator.h"

AddrGenerator::AddrGenerator(Context &c, std::ostream &os) : c(c), os(os), ok(false)
{
}

void AddrGenerator::visit(IdNode &node)
{
    if(!node.getProperty("sym"))
    {
        throw Error(node.location(), "addressable expected - ", NameVisitors::prettyName(&node));
    }

    auto sym = node.property<const Sym*>("sym");

    if(node.parent)
    {
        if(TypeVisitor::type(c, node.parent.get())->ptr)
        {
            throw Error(node.location(), "cannot access member via pointer - ", NameVisitors::prettyName(&node));
        }

        node.parent->accept(*this);

        os << "    push size(" << sym->property<std::size_t>("offset") << ");\n";
        os << "    add size;\n";

        ok = true;
    }
    else
    {
        if(sym->type() == Sym::Type::Func)
        {
            os << "    push &\"" << sym->fullname() << sym->property<const Type*>("type")->text() << "\";\n";
            ok = true;
        }
        else if(sym->type() == Sym::Type::Var)
        {
            os << "    push &\"" << sym->fullname() << "\";\n";
            ok = true;
        }
    }
}

void AddrGenerator::visit(CallNode &node)
{
    auto t = TypeVisitor::type(c, node.target.get());
    if(t->function())
    {
    }
    else
    {
        auto temp = node.property<std::string>("temp");

        ExprGenerator::generate(c, os, node);

        os << "    push &\"" << temp << "\";\n";
        ok = true;
    }
}

void AddrGenerator::visit(DerefNode &node)
{
    ExprGenerator::generate(c, os, *(node.expr.get()));
    ok = true;
}

void AddrGenerator::generate(Context &c, std::ostream &os, Node &node)
{
    AddrGenerator ag(c, os);
    node.accept(ag);

    if(!ag.result())
    {
        throw Error(node.location(), "addressable expected - ", NameVisitors::prettyName(&node));
    }
}
