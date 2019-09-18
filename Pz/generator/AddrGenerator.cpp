#include "AddrGenerator.h"

#include "framework/Error.h"

#include "nodes/IdNode.h"

#include "syms/Sym.h"

AddrGenerator::AddrGenerator(Context &c, std::ostream &os) : c(c), os(os), ok(false)
{
}

void AddrGenerator::visit(IdNode &node)
{
    auto sym = node.property<Sym*>("sym");

    if(sym->type() == Sym::Type::Var)
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
