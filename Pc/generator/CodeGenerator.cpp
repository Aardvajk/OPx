#include "CodeGenerator.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ExprNode.h"
#include "nodes/ReturnNode.h"

#include "generator/ExprGenerator.h"

CodeGenerator::CodeGenerator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void CodeGenerator::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n.accept(*this);
    }
}

void CodeGenerator::visit(ScopeNode &node)
{
    auto g = c.tree.open(node.sym);
    node.block->accept(*this);
}

void CodeGenerator::visit(ExprNode &node)
{
    auto sz = ExprGenerator::generate(c, os, *node.node);
    os << "    pop " << sz << ";\n";
}

void CodeGenerator::visit(ReturnNode &node)
{
    auto sz = ExprGenerator::generate(c, os, *node.expr);
    os << "    push &\"@ret\";\n";
    os << "    store " << sz << ";\n";
    os << "    pop " << sz << ";\n";
    os << "    jmp \"#end_function\";\n";
}
