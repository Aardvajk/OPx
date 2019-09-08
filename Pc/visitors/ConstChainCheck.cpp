#include "ConstChainCheck.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/CallNode.h"

#include "types/Type.h"

#include "visitors/TypeVisitor.h"

ConstChainCheck::ConstChainCheck(Context &c) : c(c), r(false)
{
}

void ConstChainCheck::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent->accept(*this);
    }

    if(TypeVisitor::type(c, &node)->constant)
    {
        r = true;
    }
}

void ConstChainCheck::visit(CallNode &node)
{
    auto t = TypeVisitor::type(c, node.target.get());

    if(t->function() && t->returnType->constant)
    {
        r = true;
    }
}
