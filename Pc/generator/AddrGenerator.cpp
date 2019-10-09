#include "AddrGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/CallNode.h"
#include "nodes/ProxyCallNode.h"
#include "nodes/ConstructNode.h"
#include "nodes/DerefNode.h"
#include "nodes/ThisNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include "generator/ExprGenerator.h"

AddrGenerator::AddrGenerator(Context &c, std::ostream &os) : c(c), os(os), ok(false)
{
}

void AddrGenerator::visit(IdNode &node)
{
    auto sym = node.property<Sym*>("sym");

    if(node.parent && sym->findProperty("member").value<bool>())
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

void AddrGenerator::visit(CallNode &node)
{
    ExprGenerator::generate(c, os, &node);
    ok = true;
}

void AddrGenerator::visit(ProxyCallNode &node)
{
    ExprGenerator::generate(c, os, &node);
    ok = true;
}

void AddrGenerator::visit(ConstructNode &node)
{
    ExprGenerator::generate(c, os, &node);
    ok = true;
}

void AddrGenerator::visit(DerefNode &node)
{
    ExprGenerator::generate(c, os, node.expr.get());
    ok = true;
}

void AddrGenerator::visit(ThisNode &node)
{
    os << "    push \"" << c.tree.current()->container()->fullname() << ".this\";\n";
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
