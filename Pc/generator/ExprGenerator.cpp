#include "ExprGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/IntLiteralNode.h"
#include "nodes/CallNode.h"

ExprGenerator::ExprGenerator(Context &c, std::ostream &os) : c(c), os(os), sz(0)
{
}

void ExprGenerator::visit(IntLiteralNode &node)
{
    os << "    push int(" << node.value << ");\n";
    sz = sizeof(int);
}

void ExprGenerator::visit(CallNode &node)
{
    auto sym = node.property("sym").to<const Sym*>();
    auto type = sym->property("type").to<const Type*>();

    sz = c.assertSize(node.target->location(), type->returnType.get());

    os << "    allocs " << sz << ";\n";

    for(auto &a: node.args)
    {
        ExprGenerator eg(c, os);
        a.accept(eg);
    }

    os << "    push &\"" << sym->fullname() << type->text() << "\";\n";
    os << "    call;\n";
}
