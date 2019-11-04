#include "ExprGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/ProxyCallNode.h"
#include "nodes/ConstructNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/DerefNode.h"
#include "nodes/ThisNode.h"
#include "nodes/AssignNode.h"
#include "nodes/UnaryNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/LogicalNode.h"
#include "nodes/IncDecNodes.h"
#include "nodes/CommaNode.h"
#include "nodes/InlineVarNode.h"
#include "nodes/TernaryNode.h"
#include "nodes/TypeCastNode.h"

#include "generator/CommonGenerator.h"
#include "generator/AddrGenerator.h"
#include "generator/FuncGenerator.h"

#include "operators/MathOperators.h"
#include "operators/CompareOperators.h"

#include "visitors/TypeVisitor.h"

#include "types/Type.h"

#include <pcx/indexed_range.h>
#include <pcx/scoped_push.h>

namespace
{

template<typename T> std::size_t generateIncDec(Context &c, std::ostream &os, T &node, bool pre)
{
    auto type = TypeVisitor::assertType(c, node.expr.get());

    auto pr = Primitive::toString(type->primitiveType());
    auto size = Type::assertSize(node.location(), type);

    if(!pre)
    {
        ExprGenerator::generate(c, os, node.expr.get());
    }

    ExprGenerator::generate(c, os, node.expr.get());
    os << "    push " << pr << "(1);\n";

    os << "    " << (node.token.type() == Token::Type::Inc ? "add" : "sub") << " " << pr << ";\n";

    AddrGenerator::generate(c, os, node.expr.get());
    os << "    store " << size << ";\n";

    if(!pre)
    {
        os << "    pop " << size << ";\n";
    }

    return size;
}

template<typename T> std::size_t generateCall(Context &c, std::ostream &os, T &node, Type *type)
{
    std::size_t sz = 0;

    auto rt = c.generics.convert(c, type->returnType);
    auto size = Type::assertSize(node.location(), rt);

    if(rt->primitive())
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
        auto temp = node.template property<std::string>("temp");

        os << "    push &\"" << temp << "\";\n";
        info->tempDestructs.push_back(TempDestruct(temp, rt));

        sz = sizeof(std::size_t);
    }

    for(std::size_t i = 0; i < node.params.size(); ++i)
    {
        CommonGenerator::generateParameter(c, os, node.params[i].get(), c.generics.convert(c, type->args[i]));
    }

    return sz;
}

}

ExprGenerator::ExprGenerator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void ExprGenerator::visit(IdNode &node)
{
    auto sym = node.property<Sym*>("sym");

    if(sym->type() == Sym::Type::Func)
    {
        if(node.generics.empty())
        {
            os << "    push &\"" << sym->funcname() << "\";\n";
        }
        else
        {
            os << "    push &\"" << Generic::funcName(c, sym, node.property<std::vector<Type*> >("generics")) << "\";\n";
        }

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

        if(!c.option("O", "elide_no_effect_ops") || *sz)
        {
            os << "    load " << *sz << ";\n";
        }
        else
        {
            os << "    pop " << sizeof(std::size_t) << ";\n";
        }
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

void ExprGenerator::visit(SizeLiteralNode &node)
{
    os << "    push size(" << node.value << ");\n";
    sz = c.tree.root()->child("std")->child("size")->property<std::size_t>("size");
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

    auto gp = pcx::scoped_push(c.generics, GenericParams(node.target->findProperty("generics").value<std::vector<Type*> >()));
    sz = generateCall(c, os, node, type);

    ExprGenerator::generate(c, os, node.target.get());
    os << "    call;\n";
}

void ExprGenerator::visit(ProxyCallNode &node)
{
    auto type = node.sym->property<Type*>("type");

    sz = generateCall(c, os, node, type);

    os << "    push &\"" << node.sym->funcname() << "\";\n";
    os << "    call;\n";
}

void ExprGenerator::visit(ConstructNode &node)
{
    if(node.type->primitive())
    {
        auto pt = node.type->primitiveType();

        if(node.params.empty())
        {
            os << "    push " << Primitive::toString(pt) << "(0);\n";
        }
        else
        {
            auto et = TypeVisitor::assertType(c, node.params.front().get());

            ExprGenerator::generate(c, os, node.params.front().get());

            if(et->primitiveType() != pt)
            {
                os << "    convert " << Primitive::toString(et->primitiveType()) << " " << Primitive::toString(pt) << ";\n";
            }
        }

        sz = Type::assertSize(node.location(), node.type);
    }
    else
    {
        auto temp = node.property<std::string>("temp");
        auto flag = node.findProperty("temp_ps_flag").value<std::string>();

        os << "    push &\"" << temp << "\";\n";

        auto type = TypeVisitor::assertType(c, node.target.get());

        for(auto p: pcx::indexed_range(node.params))
        {
            CommonGenerator::generateParameter(c, os, p.value.get(), type->args[p.index]);
        }

        ExprGenerator::generate(c, os, node.target.get());
        os << "    call;\n";

        if(!flag.empty())
        {
            os << "    setf \"" << flag << "\";\n";
        }

        os << "    push &\"" << temp << "\";\n";

        auto info = c.tree.current()->container()->property<FuncInfo*>("info");
        info->tempDestructs.push_back(TempDestruct(temp, node.type, flag));

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

    if(!c.option("O", "elide_no_effect_ops") || *sz)
    {
        os << "    load " << *sz << ";\n";
    }
    else
    {
        os << "    pop " << sizeof(std::size_t) << ";\n";
    }
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
        case Token::Type::Neq:
        case Token::Type::Lt:
        case Token::Type::LtEq:
        case Token::Type::Gt:
        case Token::Type::GtEq: sz = CompareOperators::generate(c, os, node); break;

        default: break;
    }
}

void ExprGenerator::visit(LogicalNode &node)
{
    auto info = c.tree.current()->container()->property<FuncInfo*>("info");

    if(node.token.type() == Token::Type::And)
    {
        auto l0 = info->nextLabelQuoted();
        auto l1 = info->nextLabelQuoted();

        CommonGenerator::generateBooleanExpression(c, os, node.left.get());
        os << "    jmp ifz " << l0 << ";\n";

        CommonGenerator::generateBooleanExpression(c, os, node.right.get());
        os << "    jmp ifz " << l0 << ";\n";

        os << "    push char(1);\n";
        os << "    jmp " << l1 << ";\n";

        os << l0 << ":\n";
        os << "    push char(0);\n";
        os << l1 << ":\n";
    }
    else
    {
        auto l0 = info->nextLabelQuoted();
        auto l1 = info->nextLabelQuoted();
        auto l2 = info->nextLabelQuoted();

        CommonGenerator::generateBooleanExpression(c, os, node.left.get());
        os << "    not char;\n";
        os << "    jmp ifz " << l0 << ";\n";

        CommonGenerator::generateBooleanExpression(c, os, node.right.get());
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

void ExprGenerator::visit(PreIncDecNode &node)
{
    sz = generateIncDec(c, os, node, true);
}

void ExprGenerator::visit(PostIncDecNode &node)
{
    sz = generateIncDec(c, os, node, false);
}

void ExprGenerator::visit(CommaNode &node)
{
    auto size = ExprGenerator::generate(c, os, node.left.get());

    if(!c.option("O", "elide_no_effect_ops") || size)
    {
        os << "    pop " << size << ";\n";
    }

    sz = ExprGenerator::generate(c, os, node.right.get());
}

void ExprGenerator::visit(InlineVarNode &node)
{
    Visitor::visit<FuncGenerator>(node.body.get(), c, os);
    sz = 0;
}

void ExprGenerator::visit(TernaryNode &node)
{
    auto info = c.tree.current()->container()->property<FuncInfo*>("info");

    auto l0 = info->nextLabelQuoted();
    auto l1 = info->nextLabelQuoted();

    CommonGenerator::generateBooleanExpression(c, os, node.expr.get());
    os << "    jmp ifz " << l0 << ";\n";

    sz = ExprGenerator::generate(c, os, node.left.get());
    os << "    jmp " << l1 << ";\n";

    os << l0 << ":\n";

    ExprGenerator::generate(c, os, node.right.get());

    os << l1 << ":\n";
}

void ExprGenerator::visit(TypeCastNode &node)
{
    sz = ExprGenerator::generate(c, os, node.expr.get());
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
