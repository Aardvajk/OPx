#include "ExprGenerator.h"

#include "application/Context.h"

#include "nodes/IntLiteralNode.h"
#include "nodes/CallNode.h"

#include "visitors/NameVisitors.h"

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
//    auto syms = node.target->property("syms").to<std::vector<SymResult> >();
}
