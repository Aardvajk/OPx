#include "CompareOperators.h"

#include "common/Primitive.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BinaryNode.h"

#include "generator/ExprGenerator.h"

#include "visitors/TypeVisitor.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

namespace
{

inline std::ostream &operator<<(std::ostream &os, Primitive::Type v){ return os << Primitive::toString(v); }

void generateEq(Context &c, std::ostream &os, Primitive::Type &pt)
{
    os << "    sub " << pt << ";\n";
    os << "    not " << pt << ";\n";
}

void generateNeq(Context &c, std::ostream &os, Primitive::Type &pt)
{
    os << "    sub " << pt << ";\n";
}

}

std::size_t CompareOperators::generate(Context &c, std::ostream &os, BinaryNode &node)
{
    auto lt = TypeVisitor::assertType(c, node.left.get());
    auto rt = TypeVisitor::assertType(c, node.right.get());

    if(!TypeCompare(c).compatible(lt, rt))
    {
        throw Error(node.location(), "invalid comparison - ", node.left->description(), " and ", node.right->description());
    }

    ExprGenerator::generate(c, os, node.left.get());
    ExprGenerator::generate(c, os, node.right.get());

    auto pt = lt->primitiveType();

    switch(node.token.type())
    {
        case Token::Type::Eq: generateEq(c, os, pt); break;
        case Token::Type::Neq: generateNeq(c, os, pt); break;

        default: break;
    }

    if(pt != Primitive::Type::Char)
    {
        os << "    convert " << pt << " char;\n";
    }

    return c.types.boolType()->size();
}
