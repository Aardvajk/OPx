#include "LocalsGenerator.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/VarNode.h"
#include "nodes/WhileNode.h"
#include "nodes/IfNode.h"
#include "nodes/ForNode.h"
#include "nodes/InlineVarNode.h"

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
    if(!node.findProperty("globalinit").value<bool>())
    {
        auto s = node.property<Sym*>("sym");
        os << "    var \"" << s->fullname() << "\":" << Type::assertSize(node.location(), s->property<Type*>("type")) << ";\n";
    }
}

void LocalsGenerator::visit(WhileNode &node)
{
    node.body->accept(*this);
}

void LocalsGenerator::visit(IfNode &node)
{
    node.body->accept(*this);

    if(node.elseBody)
    {
        node.elseBody->accept(*this);
    }
}

void LocalsGenerator::visit(ForNode &node)
{
    if(node.init)
    {
        node.init->accept(*this);
    }

    node.body->accept(*this);
}

void LocalsGenerator::visit(InlineVarNode &node)
{
    node.body->accept(*this);
}
