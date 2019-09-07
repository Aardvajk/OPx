#include "CommonGenerator.h"

#include "framework/Error.h"

#include "common/Primitive.h"

#include "application/Context.h"

#include "generator/ExprGenerator.h"
#include "generator/AddrGenerator.h"

#include "nodes/Node.h"

#include "visitors/TypeVisitor.h"
#include "visitors/NameVisitors.h"

#include "types/Type.h"
#include "types/TypeCompare.h"
#include "types/TypeLookup.h"

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

void CommonGenerator::generateParameter(Context &c, std::ostream &os, Node &node, Type *type)
{
    auto t = TypeVisitor::type(c, &node);
    if(t->constant && (type->ref || type->ptr) && !type->constant)
    {
        throw Error(node.location(), "cannot pass const as mutable - ", NameVisitors::prettyName(&node));
    }

    if(type->primitive() || type->ref)
    {
        auto tp = node.getProperty("temp_literal");

        if(type->ref && tp)
        {
            auto temp = tp.to<std::string>();
            auto sz = c.assertSize(node.location(), TypeVisitor::type(c, &node));

            ExprGenerator::generate(c, os, node);
            os << "    push &\"" << temp << "\";\n";
            os << "    store " << sz << ";\n";
            os << "    pop " << sz << ";\n";
            os << "    push &\"" << temp << "\";\n";
        }
        else
        {
            ExprGenerator::generate(c, os, node);
        }
    }
    else
    {
        auto fn = TypeLookup::assertNewCopyMethod(c, node.location(), type);

        os << "    allocs " << c.assertSize(node.location(), type) << ";\n";
        os << "    push sp;\n";
        AddrGenerator::generate(c, os, node);
        os << "    push &\"" << fn->fullname() << fn->property<const Type*>("type")->text() << "\";\n";
        os << "    call;\n";
    }
}
