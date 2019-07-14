#include "ExprGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/LiteralNodes.h"

ExprGenerator::ExprGenerator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void ExprGenerator::visit(CharLiteralNode &node)
{
    os << "    push char(" << static_cast<unsigned int>(node.value) << ");\n";
    sz = c.tree.root()->child("std")->child("char")->property("size").to<std::size_t>();
}

void ExprGenerator::visit(IntLiteralNode &node)
{
    os << "    push int(" << node.value << ");\n";
    sz = c.tree.root()->child("std")->child("int")->property("size").to<std::size_t>();
}

std::size_t ExprGenerator::generate(Context &c, std::ostream &os, Node &node)
{
    ExprGenerator eg(c, os);
    node.accept(eg);

    if(!eg.size())
    {
        throw Error(node.location(), "expr failed");
    }

    return *(eg.size());
}
