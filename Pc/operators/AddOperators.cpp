#include "AddOperators.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BinaryNode.h"

#include "visitors/TypeVisitor.h"

#include "generator/ExprGenerator.h"

#include "types/Type.h"

std::size_t AddOperators::generate(Context &c, std::ostream &os, BinaryNode &node)
{
    auto ln = node.left.get();
    auto rn = node.right.get();

    auto lt = TypeVisitor::type(c, ln);
    auto rt = TypeVisitor::type(c, rn);

    if(rt->ptr)
    {
        std::swap(ln, rn);
        std::swap(lt, rt);
    }

    if(lt->ptr)
    {
        auto dt = *lt;
        --dt.ptr;

        ExprGenerator::generate(c, os, *ln);
        ExprGenerator::generate(c, os, *rn);

        //TODO assuming a std.int for now
        os << "    convert int size;\n";

        os << "    push size(" << c.assertSize(node.location(), &dt) << ");\n";
        os << "    mul size;\n";

        os << "    add size;\n";

        return sizeof(std::size_t);
    }

    throw Error("add not implemented");
}
