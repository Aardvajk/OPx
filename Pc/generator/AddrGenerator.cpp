#include "AddrGenerator.h"

#include "application/Context.h"

#include "nodes/IdNode.h"

#include "types/Type.h"

AddrGenerator::AddrGenerator(Context &c, std::ostream &os) : c(c), os(os), ok(false)
{
}

void AddrGenerator::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent->accept(*this);

        os << "    push size(" << node.property<const Sym*>("sym")->property<std::size_t>("offset") << ");\n";
        os << "    add size;\n";

        ok = true;
    }
    else
    {
        auto s = node.property<const Sym*>("sym");

        if(s->type() == Sym::Type::Var)
        {
            os << "    push &\"" << s->fullname() << "\";\n";
            ok = true;
        }
    }
}
