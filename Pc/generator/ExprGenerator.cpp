#include "ExprGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/ConstructNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/DerefNode.h"
#include "nodes/ThisNode.h"
#include "nodes/AssignNode.h"
#include "nodes/UnaryNode.h"
#include "nodes/BinaryNode.h"

#include "generator/CommonGenerator.h"
#include "generator/AddrGenerator.h"

#include "operators/MathOperators.h"
#include "operators/CompareOperators.h"

#include "visitors/TypeVisitor.h"

#include "types/Type.h"

#include <pcx/indexed_range.h>

ExprGenerator::ExprGenerator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void ExprGenerator::visit(IdNode &node)
{
    auto sym = node.property<Sym*>("sym");

    if(sym->type() == Sym::Type::Func)
    {
        os << "    push &\"" << sym->funcname() << "\";\n";
        sz = sizeof(std::size_t);
    }
    else if(sym->findProperty("member").value<bool>())
    {
        AddrGenerator::generate(c, os, node.parent.get());

        auto o = sym->property<std::size_t>("offset");

        if(!c.option("O", "elide_no_effect_ops") || o)
        {
            os << "    push size(" << o << ");\n";
            os << "    add size;\n";
        }

        sz = Type::assertSize(node.location(), sym->property<Type*>("type"));
        os << "    load " << *sz << ";\n";
    }
    else
    {
        os << "    push \"" << sym->fullname() << "\";\n";
        sz = Type::assertSize(node.location(), sym->property<Type*>("type"));
    }
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

void ExprGenerator::visit(StringLiteralNode &node)
{
    os << "    push &\"" << node.property<std::string>("global") << "\";\n";
    sz = sizeof(std::size_t);
}

void ExprGenerator::visit(CallNode &node)
{
    auto type = TypeVisitor::assertType(c, node.target.get());
    auto size = Type::assertSize(node.location(), type->returnType);

    if(type->returnType->primitive())
    {
        if(!c.option("O", "elide_no_effect_ops") || size)
        {
            os << "    allocs " << size << ";\n";
        }

        sz = size;
    }
    else
    {
        auto info = c.tree.current()->container()->property<FuncInfo*>("info");
        auto temp = node.property<std::string>("temp");

        os << "    push &\"" << temp << "\";\n";
        info->tempDestructs.push_back(std::make_pair(temp, type->returnType));

        sz = sizeof(std::size_t);
    }

    for(auto p: pcx::indexed_range(node.params))
    {
        CommonGenerator::generateParameter(c, os, p.value.get(), type->args[p.index]);
    }

    ExprGenerator::generate(c, os, node.target.get());
    os << "    call;\n";
}

void ExprGenerator::visit(ConstructNode &node)
{
    if(node.type->primitive())
    {
        if(node.params.size() > 1)
        {
            throw Error(node.location(), "too many parameters - ", node.description());
        }

        auto pt = node.type->primitiveType();

        if(node.params.empty())
        {
            os << "    push " << Primitive::toString(pt) << "(0);\n";
        }
        else
        {
            auto et = TypeVisitor::assertType(c, node.params.front().get());
            if(et->primitive())
            {
                ExprGenerator::generate(c, os, node.params.front().get());

                if(et->primitiveType() != pt)
                {
                    os << "    convert " << Primitive::toString(et->primitiveType()) << " " << Primitive::toString(pt) << ";\n";
                }
            }
            else
            {
                throw Error(node.location(), "non-primitive conversions not supported - ", node.description());
            }
        }

        sz = Type::assertSize(node.location(), node.type);
    }
    else
    {
        auto temp = node.property<std::string>("temp");

        os << "    push &\"" << temp << "\";\n";

        auto type = TypeVisitor::assertType(c, node.target.get());

        for(auto p: pcx::indexed_range(node.params))
        {
            CommonGenerator::generateParameter(c, os, p.value.get(), type->args[p.index]);
        }

        ExprGenerator::generate(c, os, node.target.get());
        os << "    call;\n";

        os << "    push &\"" << temp << "\";\n";

        auto info = c.tree.current()->container()->property<FuncInfo*>("info");
        info->tempDestructs.push_back(std::make_pair(temp, node.type));

        sz = sizeof(std::size_t);
    }
}

void ExprGenerator::visit(AddrOfNode &node)
{
    AddrGenerator::generate(c, os, node.expr.get());
    sz = sizeof(std::size_t);
}

void ExprGenerator::visit(DerefNode &node)
{
    sz = Type::assertSize(node.expr->location(), TypeVisitor::assertType(c, &node));

    ExprGenerator::generate(c, os, node.expr.get());
    os << "    load " << *sz << ";\n";
}

void ExprGenerator::visit(ThisNode &node)
{
    os << "    push \"" << c.tree.current()->container()->fullname() << ".this\";\n";
    sz = sizeof(std::size_t);
}

void ExprGenerator::visit(AssignNode &node)
{
    if(TypeVisitor::assertType(c, &node)->primitive())
    {
        sz = ExprGenerator::generate(c, os, node.expr.get());

        AddrGenerator::generate(c, os, node.target.get());
        os << "    store " << *sz << ";\n";
    }
}

void ExprGenerator::visit(UnaryNode &node)
{
    switch(node.token.type())
    {
        case Token::Type::Sub:
        case Token::Type::Exclaim: sz = MathOperators::generateNotNeg(c, os, node); break;

        default: break;
    }
}

void ExprGenerator::visit(BinaryNode &node)
{
    switch(node.token.type())
    {
        case Token::Type::Add: sz = MathOperators::generateAdd(c, os, node); break;
        case Token::Type::Sub: sz = MathOperators::generateSub(c, os, node); break;

        case Token::Type::Star:
        case Token::Type::Div:
        case Token::Type::Mod: sz = MathOperators::generateMulDivMod(c, os, node); break;

        case Token::Type::Eq:
        case Token::Type::Neq: sz = CompareOperators::generate(c, os, node); break;

        default: break;
    }
}

std::size_t ExprGenerator::generate(Context &c, std::ostream &os, Node *node)
{
    auto r = Visitor::query<ExprGenerator, pcx::optional<std::size_t> >(node, c, os);
    if(!r)
    {
        throw Error(node->location(), "ExprGenerator failed for ", node->classname(), "node ", node->description());
    }

    return *r;
}
