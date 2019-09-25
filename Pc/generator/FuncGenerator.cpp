#include "FuncGenerator.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/VarNode.h"
#include "nodes/ExprNode.h"
#include "nodes/ReturnNode.h"

#include "generator/ExprGenerator.h"

#include "visitors/TypeVisitor.h"

#include "types/Type.h"
#include "types/TypeCompare.h"
#include "types/TypeLookup.h"

#include <pcx/range_reverse.h>

namespace
{

void processTempDestructs(Context &c, std::ostream &os, Location location)
{
    auto info = c.tree.current()->container()->property<FuncInfo*>("info");

    for(auto &t: info->tempDestructs)
    {
        auto fn = TypeLookup::assertDeleteMethod(c, location, t.second);

        os << "    push &\"" << t.first << "\";\n";
        os << "    push &\"" << fn->funcname() << "\";\n";
        os << "    call;\n";
    }

    info->tempDestructs.clear();
}

void exitScope(Context &c, std::ostream &os, Node &node)
{
    processTempDestructs(c, os, node.location());

    auto info = c.tree.current()->container()->property<FuncInfo*>("info");

    if(!info->destructs.empty() && !info->destructs.back().empty())
    {
        os << "    jmp \"#destroy_" << info->destructs.back().back()->property<Sym*>("sym")->fullname() << "\";\n";
    }
    else if(!c.option("O", "elide_unneeded_complex_returns") || c.tree.current()->container()->findProperty("complexReturns").value<bool>())
    {
        os << "    jmp \"#no_return_exit_" << c.tree.current()->fullname() << "\";\n";
    }
    else
    {
        if(c.tree.scopeDepth() > 1 || !node.block()->isLast(&node) || !c.option("O", "elide_unused_targets"))
        {
            os << "    jmp \"#end_function\";\n";
            c.tree.current()->container()->setProperty("endFunctionRef", true);
        }
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

void FuncGenerator::visit(ScopeNode &node)
{
    auto info = c.tree.current()->container()->property<FuncInfo*>("info");

    auto sg = c.tree.open(node.property<Sym*>("sym"));
    info->destructs.push_back({ });

    node.body->accept(*this);

    for(auto np: pcx::range_reverse(info->destructs.back()))
    {
        auto sym = np->property<Sym*>("sym");
        auto dm = TypeLookup::assertDeleteMethod(c, np->location(), sym->property<Type*>("type"));

        os << "\"#destroy_" << sym->fullname() << "\":\n";

        os << "    push &\"" << sym->fullname() << "\";\n";
        os << "    push &\"" << dm->funcname() << "\";\n";
        os << "    call;\n";
    }

    info->destructs.pop_back();

    if(!c.option("O", "elide_unneeded_complex_returns") || c.tree.current()->container()->findProperty("complexReturns").value<bool>())
    {
        os << "    push &\"@rf\";\n";
        os << "    load 1;\n";
        os << "    jmp ifz \"#no_return_exit_" << node.property<Sym*>("sym")->fullname() << "\";\n";

        if(info->destructs.empty() || info->destructs.back().empty())
        {
            os << "    jmp \"#end_function\";\n";
            c.tree.current()->container()->setProperty("endFunctionRef", true);
        }
        else
        {
            os << "    jmp \"#destroy_" << info->destructs.back().back()->property<Sym*>("sym")->fullname() << "\";\n";
        }

        os << "\"#no_return_exit_" << node.property<Sym*>("sym")->fullname() << "\":\n";
    }
}

void FuncGenerator::visit(VarNode &node)
{
    auto type = TypeVisitor::assertType(c, &node);
    if(node.value && type->primitive())
    {
        if(!TypeCompare(c).compatible(type, TypeVisitor::assertType(c, node.value.get())))
        {
            throw Error(node.value->location(), type->text(), " expected - ", node.value->description());
        }

        auto sz = ExprGenerator::generate(c, os, node.value.get());

        os << "    push &\"" << node.property<Sym*>("sym")->fullname() << "\";\n";
        os << "    store " << sz << ";\n";
        os << "    pop " << sz << ";\n";
    }

    if(!type->primitive())
    {
        auto info = c.tree.current()->container()->property<FuncInfo*>("info");
        info->destructs.back().push_back(&node);
    }
}

void FuncGenerator::visit(ExprNode &node)
{
    auto sz = ExprGenerator::generate(c, os, node.expr.get());

    if(!c.option("O", "elide_no_effect_ops") || sz)
    {
        os << "    pop " << sz << ";\n";
    }
}

void FuncGenerator::visit(ReturnNode &node)
{
    if(node.expr)
    {
        if(TypeVisitor::assertType(c, node.expr.get())->primitive())
        {
            auto sz = ExprGenerator::generate(c, os, node.expr.get());

            os << "    push &\"@ret\";\n";
            os << "    store " << sz << ";\n";
            os << "    pop " << sz << ";\n";
        }
    }

    if(!c.option("O", "elide_unneeded_complex_returns") || c.tree.current()->container()->findProperty("complexReturns").value<bool>())
    {
        os << "    setf \"@rf\";\n";
    }

    exitScope(c, os, node);
}
