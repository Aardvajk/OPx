#include "ByteListGenerator.h"

#include "nodes/LiteralNodes.h"

ByteListGenerator::ByteListGenerator(Context &c, std::ostream &os) : c(c), os(os), ok(false)
{
}

void ByteListGenerator::visit(CharLiteralNode &node)
{
    os << "char(" << static_cast<unsigned>(node.value) << ")";
    ok = true;
}

void ByteListGenerator::visit(IntLiteralNode &node)
{
    os << "int(" << node.value << ")";
    ok = true;
}

void ByteListGenerator::visit(BoolLiteralNode &node)
{
    os << "char(" << (node.value ? "1" : "0") << ")";
    ok = true;
}
