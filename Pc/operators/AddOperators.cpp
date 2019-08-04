#include "AddOperators.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BinaryNode.h"

#include "visitors/TypeVisitor.h"
#include "visitors/NameVisitors.h"

#include "generator/ExprGenerator.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

std::size_t AddOperators::generateAdd(Context &c, std::ostream &os, BinaryNode &node)
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

    ExprGenerator::generate(c, os, *ln);
    ExprGenerator::generate(c, os, *rn);

    if(lt->ptr)
    {
        auto dt = *lt;
        --dt.ptr;

        if(TypeCompare::exact(rt, c.types.intType()))
        {
            os << "    convert int size;\n";
        }
        else if(!TypeCompare::exact(rt, c.types.sizeType()))
        {
            throw Error(node.location(), "invalid pointer addition - ", NameVisitors::prettyName(rn));
        }

        os << "    push size(" << c.assertSize(node.location(), &dt) << ");\n";
        os << "    mul size;\n";

        os << "    add size;\n";

        return sizeof(std::size_t);
    }

    throw Error("internal error - add not supported");
}
