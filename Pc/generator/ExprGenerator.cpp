#include "ExprGenerator.h"

#include "framework/Error.h"

#include "common/Primitive.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/AssignNode.h"
#include "nodes/ThisNode.h"
#include "nodes/DerefNode.h"
#include "nodes/UnaryNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/PrimitiveCastNode.h"
#include "nodes/LogicalNode.h"

#include "visitors/TypeVisitor.h"
#include "visitors/NameVisitors.h"

#include "generator/CommonGenerator.h"
#include "generator/AddrGenerator.h"

#include "operators/MathOperators.h"
#include "operators/CompareOperators.h"

#include "types/Type.h"
#include "types/TypeLookup.h"

ExprGenerator::ExprGenerator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void ExprGenerator::visit(IdNode &node)
{
    auto s = node.property<const Sym*>("sym");
    auto t = s->property<const Type*>("type");

    if(s->type() == Sym::Type::Func)
    {
        os << "    push &\"" << s->fullname() << t->text() << "\";\n";
    }
    else if(s->type() == Sym::Type::Class)
    {
        throw Error(node.location(), "invalid syntax - ", s->fullname());
    }
    else
    {
        if(s->getProperty("member").value<bool>() && node.parent)
        {
            if(TypeVisitor::type(c, node.parent.get())->ptr)
            {
                throw Error(node.parent->location(), "cannot access member via pointer - ", NameVisitors::prettyName(node.parent.get()));
            }

            AddrGenerator::generate(c, os, *node.parent);

            os << "    push size(" << s->property<std::size_t>("offset") << ");\n";
            os << "    add size;\n";
            os << "    load " << c.assertSize(node.location(), t) << ";\n";
        }
        else
        {
            if(t->sub)
            {
                os << "    push &\"" << s->fullname() << "\";\n";
            }
            else
            {
                os << "    push \"" << s->fullname() << "\";\n";
            }
        }
    }

    sz = c.assertSize(node.location(), t);
}

void ExprGenerator::visit(NullLiteralNode &node)
{
    sz = 0;
}

void ExprGenerator::visit(CharLiteralNode &node)
{
    os << "    push char(" << static_cast<unsigned int>(node.value) << ");\n";
    sz = c.tree.root()->child("std")->child("char")->property<std::size_t>("size");
}

void ExprGenerator::visit(IntLiteralNode &node)
{
    os << "    push int(" << node.value << ");\n";
    sz = c.tree.root()->child("std")->child("int")->property<std::size_t>("size");
}

void ExprGenerator::visit(BoolLiteralNode &node)
{
    os << "    push char(" << (node.value ? 1 : 0) << ");\n";
    sz = c.tree.root()->child("std")->child("bool")->property<std::size_t>("size");
}

void ExprGenerator::visit(SizeLiteralNode &node)
{
    os << "    push size(" << node.value << ");\n";
    sz = c.tree.root()->child("std")->child("size")->property<std::size_t>("size");
}

void ExprGenerator::visit(StringLiteralNode &node)
{
    os << "    push &\"" << node.property<std::string>("global") << "\";\n";
    sz = sizeof(std::size_t);
}

void ExprGenerator::visit(CallNode &node)
{
    auto t = TypeVisitor::type(c, node.target.get());

    if(t->function())
    {
        auto rs = c.assertSize(node.location(), t->returnType);

        if(rs)
        {
            os << "    allocs " << rs << ";\n";
        }

        for(auto &p: node.params)
        {
            ExprGenerator::generate(c, os, *p.get());
        }

        ExprGenerator::generate(c, os, *node.target);

        os << "    call;\n";

        sz = rs;
    }
    else
    {
        std::vector<Type*> pv;
        for(auto &p: node.params)
        {
            pv.push_back(TypeVisitor::type(c, p.get()));
        }

        auto fn = TypeLookup::assertNewMethod(c, node.location(), t, pv);

        auto rs = c.assertSize(node.location(), t);

        if(rs)
        {
            os << "    allocs " << rs << ";\n";
        }

        os << "    push sp;\n";

        for(auto &p: node.params)
        {
            ExprGenerator::generate(c, os, *p.get());
        }

        os << "    push &\"" << fn->fullname() << fn->property<const Type*>("type")->text() << "\";\n";
        os << "    call;\n";

        sz = rs;
    }
}

void ExprGenerator::visit(AddrOfNode &node)
{
    AddrGenerator::generate(c, os, *node.expr);
    sz = sizeof(std::size_t);
}

void ExprGenerator::visit(AssignNode &node)
{
    auto s = ExprGenerator::generate(c, os, *node.expr);

    AddrGenerator::generate(c, os, *node.target);

    os << "    store " << s << ";\n";

    sz = s;
}

void ExprGenerator::visit(ThisNode &node)
{
    os << "    push \"" << c.tree.current()->container()->fullname() << ".this\";\n";
    sz = sizeof(std::size_t);
}

void ExprGenerator::visit(DerefNode &node)
{
    auto s = c.assertSize(node.expr->location(), TypeVisitor::type(c, &node));

    ExprGenerator::generate(c, os, *node.expr);

    os << "    load " << s << ";\n";
    sz = s;
}

void ExprGenerator::visit(UnaryNode &node)
{
    switch(node.op)
    {
        case Operators::Type::Not:
        case Operators::Type::Neg: sz = MathOperators::generateNotNeg(c, os, node); break;

        default: break;
    }
}

void ExprGenerator::visit(BinaryNode &node)
{
    switch(node.op)
    {
        case Operators::Type::Add: sz = MathOperators::generateAdd(c, os, node); break;
        case Operators::Type::Sub: sz = MathOperators::generateSub(c, os, node); break;

        case Operators::Type::Mul:
        case Operators::Type::Div:
        case Operators::Type::Mod: sz = MathOperators::generateMulDivMod(c, os, node); break;

        case Operators::Type::Eq:
        case Operators::Type::Neq:
        case Operators::Type::Lt:
        case Operators::Type::LtEq:
        case Operators::Type::Gt:
        case Operators::Type::GtEq: sz = CompareOperators::generate(c, os, node); break;

        default: break;
    }
}

void ExprGenerator::visit(PrimitiveCastNode &node)
{
    std::string pt = Primitive::toString(node.type->primitiveType());

    if(node.expr)
    {
        ExprGenerator::generate(c, os, *node.expr);

        os << "    convert " << Primitive::toString(TypeVisitor::type(c, node.expr.get())->primitiveType()) << " " << pt << ";\n";
    }
    else
    {
        os << "    push " << pt << "(0);\n";
    }

    sz = c.assertSize(node.location(), node.type);
}

void ExprGenerator::visit(LogicalNode &node)
{
    if(node.op == Operators::Type::And)
    {
        auto l0 = c.nextLabelQuoted();
        auto l1 = c.nextLabelQuoted();

        CommonGenerator::generateBooleanExpression(c, os, *node.left);
        os << "    jmp ifz " << l0 << ";\n";

        CommonGenerator::generateBooleanExpression(c, os, *node.right);
        os << "    jmp ifz " << l0 << ";\n";

        os << "    push char(1);\n";
        os << "    jmp " << l1 << ";\n";

        os << l0 << ":\n";
        os << "    push char(0);\n";
        os << l1 << ":\n";
    }
    else
    {
        auto l0 = c.nextLabelQuoted();
        auto l1 = c.nextLabelQuoted();
        auto l2 = c.nextLabelQuoted();

        CommonGenerator::generateBooleanExpression(c, os, *node.left);
        os << "    not char;\n";
        os << "    jmp ifz " << l0 << ";\n";

        CommonGenerator::generateBooleanExpression(c, os, *node.right);
        os << "    jmp ifz " << l1 << ";\n";

        os << l0 << ":\n";
        os << "    push char(1);\n";
        os << "    jmp " << l2 << ";\n";

        os << l1 << ":\n";
        os << "    push char(0);\n";

        os << l2 << ":\n";
    }

    sz = c.types.boolType()->size();
}

std::size_t ExprGenerator::generate(Context &c, std::ostream &os, Node &node)
{
    ExprGenerator eg(c, os);
    node.accept(eg);

    if(!eg.size())
    {
        throw Error(node.location(), "expr failed");
    }

    return *(eg.size());
}
