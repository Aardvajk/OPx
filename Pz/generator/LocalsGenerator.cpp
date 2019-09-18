#include "LocalsGenerator.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/VarNode.h"

#include "types/Type.h"

LocalsGenerator::LocalsGenerator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void LocalsGenerator::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void LocalsGenerator::visit(ScopeNode &node)
{
    auto sg = c.tree.open(node.property<Sym*>("sym"));
    node.body->accept(*this);
}

void LocalsGenerator::visit(VarNode &node)
{
    auto s = node.property<Sym*>("sym");
    os << "    var \"" << s->fullname() << "\":" << Type::assertSize(node.location(), s->property<Type*>("type")) << ";\n";
}
