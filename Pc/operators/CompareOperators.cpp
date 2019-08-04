#include "CompareOperators.h"

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

    ExprGenerator::generate(c, os, *node.left);
    ExprGenerator::generate(c, os, *node.right);

    if(lt->ptr && rt->ptr)
    {
        os << "    sub size;\n";

        if(node.op == Operators::Type::Eq)
        {
            os << "    not size;\n";
        }

        os << "    convert size char;\n";

        return c.types.boolType()->size();
    }

    throw Error("internal error - comparison not supported");
}
