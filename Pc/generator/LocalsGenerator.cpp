#include "LocalsGenerator.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/VarNode.h"

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

void LocalsGenerator::visit(ScopeNode &node)
{
    node.body->accept(*this);
}

void LocalsGenerator::visit(VarNode &node)
{
    auto sym = node.property("sym").to<const Sym*>();
    os << "    var \"" << sym->fullname() << "\":" << c.assertSize(node.location(), sym->property("type").to<const Type*>()) << ";\n";
}