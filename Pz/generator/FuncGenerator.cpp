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

void FuncGenerator::visit(ScopeNode &node)
{
    auto sg = c.tree.open(node.property<Sym*>("sym"));
    node.body->accept(*this);
}

void FuncGenerator::visit(VarNode &node)
{
    if(node.value && TypeVisitor::assertType(c, &node)->primitive())
    {
        auto sz = ExprGenerator::generate(c, os, node.value.get());

        os << "    push &\"" << node.property<Sym*>("sym")->fullname() << "\";\n";
        os << "    store " << sz << ";\n";
        os << "    pop " << sz << ";\n";
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
}
