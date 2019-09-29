#include "QueryVisitors.h"

#include "nodes/IdNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ClassNode.h"
#include "nodes/CallNode.h"
#include "nodes/InitNode.h"
#include "nodes/ThisNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include "visitors/TypeVisitor.h"

QueryVisitors::DirectType::DirectType() : r(nullptr)
{
}

void QueryVisitors::DirectType::visit(IdNode &node)
{
    auto sym = node.property<Sym*>("sym");
    if(sym->type() == Sym::Type::Class)
    {
        r = sym->property<Type*>("type");
    }
}

QueryVisitors::GetParent::GetParent()
{
}

void QueryVisitors::GetParent::visit(IdNode &node)
{
    r = node.parent;
}

QueryVisitors::GetBlockNode::GetBlockNode() : r(nullptr)
{
}

void QueryVisitors::GetBlockNode::visit(BlockNode &node)
{
    r = &node;
}

void QueryVisitors::GetBlockNode::visit(FuncNode &node)
{
    if(node.body)
    {
        node.body->accept(*this);
    }
}

void QueryVisitors::GetBlockNode::visit(ScopeNode &node)
{
    node.body->accept(*this);
}

void QueryVisitors::GetBlockNode::visit(ClassNode &node)
{
    node.body->accept(*this);
}

QueryVisitors::GetConstructNode::GetConstructNode() : r(nullptr)
{
}

void QueryVisitors::GetConstructNode::visit(ConstructNode &node)
{
    r = &node;
}

QueryVisitors::InitNodeMap::InitNodeMap(std::unordered_map<std::string, NodePtr> &m, NodePtr &n) : m(m), n(n)
{
}

void QueryVisitors::InitNodeMap::visit(InitNode &node)
{
    m[node.name] = n;
}


QueryVisitors::IsMutable::IsMutable(Context &c) : c(c), r(true)
{
}

void QueryVisitors::IsMutable::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent->accept(*this);
    }

    if(r)
    {
        auto t = TypeVisitor::queryType(c, &node);

        if(t && t->constant)
        {
            r = false;
        }
    }
}

void QueryVisitors::IsMutable::visit(CallNode &node)
{
    auto t = TypeVisitor::assertType(c, node.target.get());

    if(t->returnType && t->returnType->constant)
    {
        r = false;
    }
}

void QueryVisitors::IsMutable::visit(ThisNode &node)
{
    if(TypeVisitor::assertType(c, &node)->constant)
    {
        r = false;
    }
}
