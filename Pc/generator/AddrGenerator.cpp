#include "AddrGenerator.h"

#include "application/Context.h"

#include "nodes/IdNode.h"

AddrGenerator::AddrGenerator(Context &c, std::ostream &os) : c(c), os(os), ok(false)
{
}

void AddrGenerator::visit(IdNode &node)
{
    auto s = node.property<const Sym*>("sym");

    if(s->type() == Sym::Type::Var)
    {
        os << "    push &\"" << s->fullname() << "\";\n";
        ok = true;
    }
}
