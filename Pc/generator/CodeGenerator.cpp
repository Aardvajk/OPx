#include "CodeGenerator.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ExprNode.h"

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
    node.body->accept(*this);
}

void CodeGenerator::visit(ExprNode &node)
{
    auto sz = ExprGenerator::generate(c, os, *node.expr);
    os << "    pop " << sz << ";\n";
}
