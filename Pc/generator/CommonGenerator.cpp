#include "CommonGenerator.h"

#include "framework/Error.h"

#include "common/Primitive.h"

#include "application/Context.h"

#include "generator/ExprGenerator.h"

#include "visitors/TypeVisitor.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

void CommonGenerator::generateBooleanExpression(Context &c, std::ostream &os, Node &node)
{
    auto et = TypeVisitor::type(c, &node);

    ExprGenerator::generate(c, os, node);

    if(!TypeCompare::exact(et, c.types.charType()) && !TypeCompare::exact(et, c.types.boolType()))
    {
        if(et->primitive())
        {
            os << "    convert " << Primitive::toString(et->primitiveType()) << " char;\n";
        }
        else
        {
            throw Error("internal error - boolean cast of non-primitive not supported");
        }
    }
}
