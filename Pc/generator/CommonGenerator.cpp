#include "CommonGenerator.h"

#include "application/Context.h"

#include "nodes/Node.h"

#include "syms/Sym.h"

#include "types/Type.h"
#include "types/TypeLookup.h"

#include "generator/ExprGenerator.h"
#include "generator/AddrGenerator.h"

#include "visitors/TypeVisitor.h"

void CommonGenerator::generateParameter(Context &c, std::ostream &os, Node *node, Type *type)
{
    if(type->primitive())
    {
        if(auto t = node->findProperty("temp_literal"))
        {
            auto temp = t.to<std::string>();
            auto size = Type::assertSize(node->location(), TypeVisitor::assertType(c, node));

            ExprGenerator::generate(c, os, node);
            os << "    push &\"" << temp << "\";\n";
            os << "    store " << size << ";\n";
            os << "    pop " << size << ";\n";
            os << "    push &\"" << temp << "\";\n";
        }
        else
        {
            ExprGenerator::generate(c, os, node);
        }
    }
    else
    {
        auto cm = TypeLookup::assertCopyMethod(c, node->location(), type);

        auto size = Type::assertSize(node->location(), type);

        if(!c.option("O", "elide_no_effect_ops") || size)
        {
            os << "    allocs " << size << ";\n";
        }

        os << "    push sp;\n";
        AddrGenerator::generate(c, os, node);
        os << "    push &\"" << cm->funcname() << "\";\n";
        os << "    call;\n";
    }
}
