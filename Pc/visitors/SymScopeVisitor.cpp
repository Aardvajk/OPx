#include "SymScopeVisitor.h"

#include "nodes/CallNode.h"
#include "nodes/ProxyCallNode.h"
#include "nodes/ConstructNode.h"
#include "nodes/ThisNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include "visitors/TypeVisitor.h"

SymScopeVisitor::SymScopeVisitor(Context &c, Sym *curr) : c(c), curr(curr)
{
}

void SymScopeVisitor::visit(CallNode &node)
{
    auto t = TypeVisitor::assertType(c, node.target.get());
    if(t->returnType && t->returnType->sym)
    {
        curr = t->returnType->sym;
    }
}

void SymScopeVisitor::visit(ProxyCallNode &node)
{
    if(node.sym)
    {
        auto type = node.sym->property<Type*>("type")->returnType;
        if(type->sym)
        {
            curr = type->sym;
        }
    }
}

void SymScopeVisitor::visit(ConstructNode &node)
{
    if(node.type->sym)
    {
        curr = node.type->sym;
    }
}

void SymScopeVisitor::visit(ThisNode &node)
{
    curr = curr->container()->parent();
}
