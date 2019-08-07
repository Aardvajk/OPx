#include "MathOperators.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/UnaryNode.h"
#include "nodes/BinaryNode.h"

#include "visitors/TypeVisitor.h"
#include "visitors/NameVisitors.h"

#include "generator/ExprGenerator.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

std::size_t MathOperators::generateAdd(Context &c, std::ostream &os, BinaryNode &node)
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

    os << "    add " << Primitive::toString(lt->primitiveType()) << ";\n";
    return c.assertSize(node.location(), lt);
}

std::size_t MathOperators::generateSub(Context &c, std::ostream &os, BinaryNode &node)
{
    auto ln = node.left.get();
    auto rn = node.right.get();

    auto lt = TypeVisitor::type(c, ln);

    ExprGenerator::generate(c, os, *ln);
    ExprGenerator::generate(c, os, *rn);

    if(lt->ptr)
    {
        throw Error("internal error - pointer subtraction not supported");
    }

    os << "    sub " << Primitive::toString(lt->primitiveType()) << ";\n";
    return c.assertSize(node.location(), lt);
}

std::size_t MathOperators::generateMulDivMod(Context &c, std::ostream &os, BinaryNode &node)
{
    auto ln = node.left.get();
    auto rn = node.right.get();

    auto lt = TypeVisitor::type(c, ln);

    ExprGenerator::generate(c, os, *ln);
    ExprGenerator::generate(c, os, *rn);

    if(lt->ptr)
    {
        throw Error(node.location(), "invalid operator - ", lt->text(), " and ", TypeVisitor::type(c, rn)->text());
    }

    std::string op;
    switch(node.op)
    {
        case Operators::Type::Mul: op = "mul"; break;
        case Operators::Type::Div: op = "div"; break;
        case Operators::Type::Mod: op = "mod"; break;

        default: break;
    }

    os << "    " << op << " " << Primitive::toString(lt->primitiveType()) << ";\n";
    return c.assertSize(node.location(), lt);
}

std::size_t MathOperators::generateNotNeg(Context &c, std::ostream &os, UnaryNode &node)
{
    auto t = TypeVisitor::type(c, node.expr.get());

    ExprGenerator::generate(c, os, *node.expr);

    if(t->ptr)
    {
        throw Error(node.location(), "invalid operator - ", t->text());
    }

    std::string op;
    switch(node.op)
    {
        case Operators::Type::Not: op = "not"; break;
        case Operators::Type::Neg: op = "neg"; break;

        default: break;
    }

    os << "    " << op << " " << Primitive::toString(t->primitiveType()) << ";\n";
    return c.assertSize(node.location(), t);
}
