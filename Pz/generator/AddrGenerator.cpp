#include "AddrGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/DerefNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include "generator/ExprGenerator.h"

AddrGenerator::AddrGenerator(Context &c, std::ostream &os) : c(c), os(os), ok(false)
{
}

void AddrGenerator::visit(IdNode &node)
{
    auto sym = node.property<Sym*>("sym");

    if(node.parent)
    {
        AddrGenerator::generate(c, os, node.parent.get());

        auto o = sym->property<std::size_t>("offset");

        if(!c.option("O", "elide_no_effect_ops") || o)
        {
            os << "    push size(" << o << ");\n";
            os << "    add size;\n";
        }

        ok = true;
    }
    else if(sym->type() == Sym::Type::Func)
    {
        os << "    push &\"" << sym->funcname() << "\";\n";
        ok = true;
    }
    else if(sym->type() == Sym::Type::Var)
    {
        os << "    push &\"" << sym->fullname() << "\";\n";
        ok = true;
    }
}

void AddrGenerator::visit(DerefNode &node)
{
    ExprGenerator::generate(c, os, node.expr.get());
    ok = true;
}

void AddrGenerator::generate(Context &c, std::ostream &os, Node *node)
{
    auto r = Visitor::query<AddrGenerator, bool>(node, c, os);
    if(!r)
    {
        throw Error(node->location(), "addressable expected - ", node->description());
    }
}
