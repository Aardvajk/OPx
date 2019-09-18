#include "ExprGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/LiteralNodes.h"

ExprGenerator::ExprGenerator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void ExprGenerator::visit(IdNode &node)
{
}

void ExprGenerator::visit(IntLiteralNode &node)
{
    os << "    push int(" << node.value << ");\n";
    sz = c.tree.root()->child("std")->child("int")->property<std::size_t>("size");
}

void ExprGenerator::visit(CallNode &node)
{
}

std::size_t ExprGenerator::generate(Context &c, std::ostream &os, Node *node)
{
    auto r = Visitor::query<ExprGenerator, pcx::optional<std::size_t> >(node, c, os);
    if(!r)
    {
        throw Error(node->location(), "ExprGenerator failed for ", node->classname(), "node ", node->description());
    }

    return *r;
}
