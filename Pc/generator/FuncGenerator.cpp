#include "FuncGenerator.h"

#include "framework/Error.h"

#include "common/Primitive.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/VarNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ExprNode.h"
#include "nodes/ReturnNode.h"
#include "nodes/WhileNode.h"
#include "nodes/IfNode.h"

#include "generator/ExprGenerator.h"
#include "generator/CommonGenerator.h"

#include "visitors/TypeVisitor.h"
#include "visitors/NameVisitors.h"

#include "types/Type.h"
#include "types/TypeCompare.h"
#include "types/TypeLookup.h"

#include <pcx/range_reverse.h>

FuncGenerator::FuncGenerator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void FuncGenerator::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void FuncGenerator::visit(VarNode &node)
{
    auto s = node.property<Sym*>("sym");

    if(!s->property<const Type*>("type")->primitive())
    {
        c.destructs.back().push_back(&node);
    }
}

void FuncGenerator::visit(ScopeNode &node)
{
    auto g = c.tree.open(node.property<Sym*>("sym"));
    c.destructs.push_back({ });

    node.body->accept(*this);

    for(auto np: pcx::range_reverse(c.destructs.back()))
    {
        auto sym = np->property<Sym*>("sym");
        auto dm = TypeLookup::assertDeleteMethod(c, np->location(), sym->property<Type*>("type"));

        os << "\"#destroy_" << sym->fullname() << "\":\n";

        os << "    push &\"" << sym->fullname() << "\";\n";
        os << "    push &\"" << dm->fullname() << dm->property<const Type*>("type")->text() << "\";\n";
        os << "    call;\n";
    }

    c.destructs.pop_back();

    os << "    push &\"@rf\";\n";
    os << "    load 1;\n";
    os << "    jmp ifz \"#no_return_exit_" << node.property<Sym*>("sym")->fullname() << "\";\n";

    if(c.destructs.empty() || c.destructs.back().empty())
    {
        os << "    jmp \"#end_function\";\n";
    }
    else
    {
        os << "    jmp \"#destroy_" << c.destructs.back().back()->property<Sym*>("sym")->fullname() << "\";\n";
    }

    os << "\"#no_return_exit_" << node.property<Sym*>("sym")->fullname() << "\":\n";
}

void FuncGenerator::visit(ExprNode &node)
{
    auto sz = ExprGenerator::generate(c, os, *node.expr);
    os << "    pop " << sz << ";\n";
}

void FuncGenerator::visit(ReturnNode &node)
{
    auto sz = ExprGenerator::generate(c, os, *node.expr);
    os << "    push &\"@ret\";\n";
    os << "    store " << sz << ";\n";
    os << "    pop " << sz << ";\n";

    os << "    setf \"@rf\";\n";

//    os << "    push char(1);\n";
//    os << "    push &\"@rf\";\n";
//    os << "    store 1;\n";
//    os << "    pop 1;\n";
}

void FuncGenerator::visit(WhileNode &node)
{
    auto l0 = c.nextLabelQuoted();
    auto l1 = c.nextLabelQuoted();

    os << l0 << ":\n";

    CommonGenerator::generateBooleanExpression(c, os, *node.expr);
    os << "    jmp ifz " << l1 << ";\n";

    node.body->accept(*this);

    os << "    jmp " << l0 << ";\n";
    os << l1 << ":\n";
}

void FuncGenerator::visit(IfNode &node)
{
    auto l0 = c.nextLabelQuoted();
    auto l1 = c.nextLabelQuoted();

    CommonGenerator::generateBooleanExpression(c, os, *node.expr);
    os << "    jmp ifz " << l0 << ";\n";

    node.body->accept(*this);
    os << "    jmp " << l1 << ";\n";

    os << l0 << ":\n";

    if(node.elseBody)
    {
        node.elseBody->accept(*this);
    }

    os << l1 << ":\n";
}
