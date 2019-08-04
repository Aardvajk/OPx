#include "FuncGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ExprNode.h"
#include "nodes/ReturnNode.h"
#include "nodes/WhileNode.h"

#include "generator/ExprGenerator.h"

#include "visitors/TypeVisitor.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

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

void FuncGenerator::visit(WhileNode &node)
{
    if(!TypeCompare::exact(TypeVisitor::type(c, node.expr.get()), c.types.boolType()))
    {
        throw Error("internal error - non-bool conditions not supported");
    }

    auto l0 = c.nextLabel();
    auto l1 = c.nextLabel();

    os << "\"" << l0 << "\":\n";
    ExprGenerator::generate(c, os, *node.expr);
    os << "    jmp ifz \"" << l1 << "\";\n";

    node.body->accept(*this);

    os << "    jmp \"" << l0 << "\";\n";
    os << "\"" << l1 << "\":\n";
}
