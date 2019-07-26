#include "AddrGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/DerefNode.h"

#include "visitors/NameVisitors.h"

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
        node.parent->accept(*this);

        os << "    push size(" << sym->property<std::size_t>("offset") << ");\n";
        os << "    add size;\n";

        ok = true;
    }
    else
    {
        if(sym->type() == Sym::Type::Var)
        {
            os << "    push &\"" << sym->fullname() << "\";\n";
            ok = true;
        }
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
