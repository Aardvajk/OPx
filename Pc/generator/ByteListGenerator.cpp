#include "ByteListGenerator.h"

#include "scanner/Lexer.h"

#include "nodes/LiteralNodes.h"

CanByteListGenerate::CanByteListGenerate() : ok(false)
{
}

void CanByteListGenerate::visit(CharLiteralNode &node)
{
    ok = true;
}

void CanByteListGenerate::visit(IntLiteralNode &node)
{
    ok = true;
}

void CanByteListGenerate::visit(BoolLiteralNode &node)
{
    ok = true;
}

void CanByteListGenerate::visit(StringLiteralNode &node)
{
    ok = true;
}

ByteListGenerator::ByteListGenerator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void ByteListGenerator::visit(CharLiteralNode &node)
{
    os << "char(" << static_cast<unsigned int>(node.value) << ")";
}

void ByteListGenerator::visit(IntLiteralNode &node)
{
    os << "int(" << node.value << ")";
}

void ByteListGenerator::visit(BoolLiteralNode &node)
{
    os << "char(" << (node.value ? "1" : "0") << ")";
}

void ByteListGenerator::visit(StringLiteralNode &node)
{
    os << "string(\"" << Lexer::encodeString(node.value) << "\"), char(0)";
}
