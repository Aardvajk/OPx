#include "AddrGenerator.h"

#include "framework/Error.h"

#include "nodes/IdNode.h"

#include "application/Context.h"

#include "syms/Sym.h"

#include "visitors/TypeVisitor.h"

#include "types/Type.h"

AddrGenerator::AddrGenerator(Context &c, std::ostream &os) : c(c), os(os), ok(false)
{
}

void AddrGenerator::visit(IdNode &node)
{
    auto sym = node.property<Sym*>("sym");

    if(node.parent)
    {
        if(TypeVisitor::assertType(c, node.parent.get())->ptr)
        {
            throw Error(node.parent->location(), "cannot access via pointer - ", node.description());
        }

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

void AddrGenerator::generate(Context &c, std::ostream &os, Node *node)
{
    auto r = Visitor::query<AddrGenerator, bool>(node, c, os);
    if(!r)
    {
        throw Error(node->location(), "addressable expected - ", node->description());
    }
}
