#include "LocalsGenerator.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/VarNode.h"
#include "nodes/ScopeNode.h"

LocalsGenerator::LocalsGenerator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void LocalsGenerator::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n.accept(*this);
    }
}

void LocalsGenerator::visit(VarNode &node)
{
    os << "    var \"" << node.sym->fullname() << "\":" << c.assertSize(node.location(), node.sym->property("type").to<const Type*>()) << ";\n";
}

void LocalsGenerator::visit(ScopeNode &node)
{
    node.block->accept(*this);
}
