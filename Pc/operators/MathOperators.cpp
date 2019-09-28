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

    auto lt = TypeVisitor::assertType(c, ln);
    auto rt = TypeVisitor::assertType(c, rn);

    if(rt->ptr)
    {
        std::swap(ln, rn);
        std::swap(lt, rt);
    }

    ExprGenerator::generate(c, os, ln);
    ExprGenerator::generate(c, os, rn);

    if(lt->ptr)
    {
        auto dt = *lt;
        --dt.ptr;

        if(TypeCompare(c).compatible(rt, c.types.intType()))
        {
            os << "    convert int size;\n";
        }
        else if(!TypeCompare(c).compatible(rt, c.types.sizeType()))
        {
            throw Error(node.location(), "invalid pointer addition - ", node.description());
        }

        os << "    push size(" << Type::assertSize(node.location(), &dt) << ");\n";
        os << "    mul size;\n";

        os << "    add size;\n";

        return sizeof(std::size_t);
    }

    if(!TypeCompare(c).compatible(lt, rt))
    {
        throw Error(node.location(), "invalid mixed-mode maths - ", node.description());
    }

    os << "    add " << Primitive::toString(lt->primitiveType()) << ";\n";
    return Type::assertSize(node.location(), lt);
}

std::size_t MathOperators::generateSub(Context &c, std::ostream &os, BinaryNode &node)
{
    auto ln = node.left.get();
    auto rn = node.right.get();

    auto lt = TypeVisitor::assertType(c, ln);

    ExprGenerator::generate(c, os, ln);
    ExprGenerator::generate(c, os, rn);

    if(lt->ptr)
    {
        throw Error("internal error - pointer subtraction not supported");
    }

    if(!TypeCompare(c).compatible(lt, TypeVisitor::assertType(c, rn)))
    {
        throw Error(node.location(), "invalid mixed-mode maths - ", node.description());
    }

    os << "    sub " << Primitive::toString(lt->primitiveType()) << ";\n";
    return Type::assertSize(node.location(), lt);
}

std::size_t MathOperators::generateMulDivMod(Context &c, std::ostream &os, BinaryNode &node)
{
    auto ln = node.left.get();
    auto rn = node.right.get();

    auto lt = TypeVisitor::assertType(c, ln);

    ExprGenerator::generate(c, os, ln);
    ExprGenerator::generate(c, os, rn);

    if(lt->ptr)
    {
        throw Error(node.location(), "invalid operator - ", node.description());
    }

    std::string op;
    switch(node.token.type())
    {
        case Token::Type::Star: op = "mul"; break;
        case Token::Type::Div: op = "div"; break;
        case Token::Type::Mod: op = "mod"; break;

        default: break;
    }

    if(!TypeCompare(c).compatible(lt, TypeVisitor::assertType(c, rn)))
    {
        throw Error(node.location(), "invalid mixed-mode maths - ", node.description());
    }

    os << "    " << op << " " << Primitive::toString(lt->primitiveType()) << ";\n";
    return Type::assertSize(node.location(), lt);
}

std::size_t MathOperators::generateNotNeg(Context &c, std::ostream &os, UnaryNode &node)
{
    auto t = TypeVisitor::assertType(c, node.expr.get());

    ExprGenerator::generate(c, os, node.expr.get());

    if(t->ptr)
    {
        throw Error(node.location(), "invalid operator - ", t->text());
    }

    std::string op;
    switch(node.token.type())
    {
        case Token::Type::Sub: op = "neg"; break;
        case Token::Type::Exclaim: op = "not"; break;

        default: break;
    }

    os << "    " << op << " " << Primitive::toString(t->primitiveType()) << ";\n";
    return Type::assertSize(node.location(), t);
}
