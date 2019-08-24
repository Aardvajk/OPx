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

void generateLt(Context &c, std::ostream &os, Primitive::Type &pt)
{
    os << "    lt " << pt << ";\n";
    pt = Primitive::Type::Char;
}

void generateLtEq(Context &c, std::ostream &os, Primitive::Type &pt)
{
    os << "    lteq " << pt << ";\n";
    pt = Primitive::Type::Char;
}

void generateGt(Context &c, std::ostream &os, Primitive::Type &pt)
{
    generateLtEq(c, os, pt);
    os << "    not char;\n";
}

void generateGtEq(Context &c, std::ostream &os, Primitive::Type &pt)
{
    generateLt(c, os, pt);
    os << "    not char;\n";
}

}

std::size_t CompareOperators::generate(Context &c, std::ostream &os, BinaryNode &node)
{
    auto lt = TypeVisitor::type(c, node.left.get());
    auto rt = TypeVisitor::type(c, node.right.get());

    if(!TypeCompare::compatible(lt, rt))
    {
        throw Error(node.location(), "invalid comparison - ", lt->text(), " and ", rt->text());
    }

    ExprGenerator::generate(c, os, *node.left);
    ExprGenerator::generate(c, os, *node.right);

    auto pt = lt->primitiveType();

    switch(node.op)
    {
        case Operators::Type::Eq: generateEq(c, os, pt); break;
        case Operators::Type::Neq: generateNeq(c, os, pt); break;

        case Operators::Type::Lt: generateLt(c, os, pt); break;
        case Operators::Type::LtEq: generateLtEq(c, os, pt); break;
        case Operators::Type::Gt: generateGt(c, os, pt); break;
        case Operators::Type::GtEq: generateGtEq(c, os, pt); break;

        default: break;
    }

    if(pt != Primitive::Type::Char)
    {
        os << "    convert " << pt << " char;\n";
    }

    return c.types.boolType()->size();
}
