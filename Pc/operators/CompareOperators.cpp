#include "CompareOperators.h"

#include "common/Primitive.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BinaryNode.h"

#include "generator/ExprGenerator.h"

#include "visitors/TypeVisitor.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

std::size_t CompareOperators::generate(Context &c, std::ostream &os, BinaryNode &node)
{
    auto lt = TypeVisitor::type(c, node.left.get());
    auto rt = TypeVisitor::type(c, node.right.get());

    if(!TypeCompare::exact(lt, rt))
    {
        throw Error(node.location(), "invalid comparison - ", lt->text(), " and ", rt->text());
    }

    ExprGenerator::generate(c, os, *node.left);
    ExprGenerator::generate(c, os, *node.right);

    std::string pt = Primitive::toString(lt->primitiveType());

    os << "    sub " << pt << ";\n";

    if(node.op == Operators::Type::Eq)
    {
        os << "    not " << pt << ";\n";
    }

    os << "    convert " << pt << " char;\n";

    return c.types.boolType()->size();
}
