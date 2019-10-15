#include "CommonGenerator.h"

#include "application/Context.h"

#include "nodes/FuncNode.h"

#include "syms/Sym.h"

#include "types/Type.h"
#include "types/TypeCompare.h"
#include "types/TypeLookup.h"

#include "generator/ExprGenerator.h"
#include "generator/AddrGenerator.h"

#include "visitors/TypeVisitor.h"
#include "visitors/QueryVisitors.h"

void CommonGenerator::generateBooleanExpression(Context &c, std::ostream &os, Node *node)
{
    auto et = TypeVisitor::assertType(c, node);

    ExprGenerator::generate(c, os, node);

    if(!TypeCompare(c).compatible(et, c.types.charType()) && !TypeCompare(c).compatible(et, c.types.boolType()))
    {
        if(et->primitive())
        {
            os << "    test " << Primitive::toString(et->primitiveType()) << ";\n";
        }
        else
        {
//            throw Error("internal error - boolean cast of non-primitive not supported");
        }
    }
}

void CommonGenerator::generateParameter(Context &c, std::ostream &os, Node *node, Type *type)
{
    if(type->primitive())
    {
        if(auto t = node->findProperty("temp_literal"))
        {
            auto temp = t.to<std::string>();
            auto size = Type::assertSize(node->location(), TypeVisitor::assertType(c, node));

            ExprGenerator::generate(c, os, node);
            os << "    push &\"" << temp << "\";\n";
            os << "    store " << size << ";\n";
            os << "    pop " << size << ";\n";
            os << "    push &\"" << temp << "\";\n";
        }
        else
        {
            ExprGenerator::generate(c, os, node);
        }
    }
    else
    {
        auto cm = TypeLookup::assertCopyMethod(c, node->location(), type);

        auto size = Type::assertSize(node->location(), type);

        if(!c.option("O", "elide_no_effect_ops") || size)
        {
            os << "    allocs " << size << ";\n";
        }

        os << "    push sp;\n";
        AddrGenerator::generate(c, os, node);
        generateAllDefaultParameters(c, os, cm);

        os << "    push &\"" << cm->funcname() << "\";\n";
        os << "    call;\n";
    }
}

void CommonGenerator::generateAllDefaultParameters(Context &c, std::ostream &os, Sym *sym)
{
    auto fn = sym->property<FuncNode*>("funcnode");
    auto no = sym->findProperty("defaults").value<std::size_t>();

    std::size_t i = fn->args.size() - no;
    while(i < fn->args.size())
    {
        auto v = Visitor::query<QueryVisitors::GetVarValue, NodePtr>(fn->args[i]);
        generateParameter(c, os, v.get(), TypeVisitor::assertType(c, v.get()));

        ++i;
    }
}
