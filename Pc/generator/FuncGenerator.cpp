#include "FuncGenerator.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ExprNode.h"
#include "nodes/ReturnNode.h"

#include "generator/ExprGenerator.h"

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
    node.body->accept(*this);
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
    os << "    jmp \"#end_function\";\n";
}
