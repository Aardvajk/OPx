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
#include "nodes/ForNode.h"
#include "nodes/BreakNode.h"

#include "generator/ExprGenerator.h"
#include "generator/CommonGenerator.h"
#include "generator/AddrGenerator.h"

#include "visitors/TypeVisitor.h"
#include "visitors/NameVisitors.h"

#include "types/Type.h"
#include "types/TypeCompare.h"
#include "types/TypeLookup.h"

#include <pcx/range_reverse.h>

namespace
{

void processTempDestructs(Context &c, std::ostream &os, Location location)
{
    for(auto &t: c.tempDestructs)
    {
        auto fn = TypeLookup::assertDeleteMethod(c, location, t.second);

        os << "    push &\"" << t.first << "\";\n";
        os << "    push &\"" << fn->fullname() << fn->property<const Type*>("type")->text() << "\";\n";
        os << "    call;\n";
    }

    c.tempDestructs.clear();
}

void exitScope(Context &c, std::ostream &os, Node &node)
{
    processTempDestructs(c, os, node.location());

    if(!c.destructs.empty() && !c.destructs.back().empty())
    {
        os << "    jmp \"#destroy_" << c.destructs.back().back()->property<Sym*>("sym")->fullname() << "\";\n";
    }
    else
    {
        os << "    jmp \"#no_return_exit_" << c.tree.current()->fullname() << "\";\n";
    }
}

}

FuncGenerator::FuncGenerator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void FuncGenerator::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
        processTempDestructs(c, os, node.location());
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
    auto sym = node.property<Sym*>("sym");

    if(auto bp = sym->getProperty("breakflag"))
    {
        os << "    clrf \"" << bp.to<std::string>() << "\";\n";
    }

    auto g = c.tree.open(sym);
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

    if(!c.option("O", "ellide_zero_ops") || sz)
    {
        os << "    pop " << sz << ";\n";
    }
}

void FuncGenerator::visit(ReturnNode &node)
{
    auto r = c.tree.current()->container()->property<Type*>("type")->returnType;

    if(r->primitive() || r->ref)
    {
        auto sz = ExprGenerator::generate(c, os, *node.expr);

        os << "    push &\"@ret\";\n";
        os << "    store " << sz << ";\n";
        os << "    pop " << sz << ";\n";
    }
    else
    {
        auto fn = TypeLookup::assertNewCopyMethod(c, node.location(), r);

        os << "    push &\"@ret\";\n";
        os << "    load 8;\n";

        AddrGenerator::generate(c, os, *node.expr);

        os << "    push &\"" << fn->fullname() << fn->property<const Type*>("type")->text() << "\";\n";
        os << "    call;\n";
    }

    os << "    setf \"@rf\";\n";

    exitScope(c, os, node);

    auto s = c.tree.current();
    std::size_t n = 0;

    while(s && s != c.tree.current()->container())
    {
        s = s->parent();
        ++n;
    }

    if(n == 1)
    {
        c.tree.current()->container()->setProperty("returned", true);
    }
}

void FuncGenerator::visit(WhileNode &node)
{
    auto l0 = c.nextLabelQuoted();
    auto l1 = c.nextLabelQuoted();

    os << l0 << ":\n";

    CommonGenerator::generateBooleanExpression(c, os, *node.expr);
    os << "    jmp ifz " << l1 << ";\n";

    node.body->accept(*this);

    auto flag = node.body->property<Sym*>("sym")->property<std::string>("breakflag");

    os << "    push &\"" << flag << "\";\n";
    os << "    load 1;\n";

    os << "    jmp ifz " << l0 << ";\n";
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

void FuncGenerator::visit(ForNode &node)
{
    auto l0 = c.nextLabelQuoted();
    auto l1 = c.nextLabelQuoted();

    if(node.init)
    {
        auto sz = ExprGenerator::generate(c, os, *node.init);
        os << "    pop " << sz << ";\n";
    }

    processTempDestructs(c, os, node.location());

    os << l0 << ":\n";

    if(node.cond)
    {
        CommonGenerator::generateBooleanExpression(c, os, *node.cond);
        os << "    jmp ifz " << l1 << ";\n";
    }

    node.body->accept(*this);

    if(node.post)
    {
        auto sz = ExprGenerator::generate(c, os, *node.post);
        os << "    pop " << sz << ";\n";
    }

    auto flag = node.body->property<Sym*>("sym")->property<std::string>("breakflag");

    os << "    push &\"" << flag << "\";\n";
    os << "    load 1;\n";

    os << "    jmp ifz " << l0 << ";\n";

    os << l1 << ":\n";
}

void FuncGenerator::visit(BreakNode &node)
{
    auto s = c.tree.current();
    while(!s->getProperty("loop").value<bool>())
    {
        s = s->parent();
    }

    os << "    setf \"" << s->property<std::string>("breakflag") << "\";\n";
    exitScope(c, os, node);
}
