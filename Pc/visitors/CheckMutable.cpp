#include "CheckMutable.h"

#include "nodes/IdNode.h"
#include "nodes/CallNode.h"
#include "nodes/ThisNode.h"

#include "types/Type.h"

#include "visitors/TypeVisitor.h"

CheckMutable::CheckMutable(Context &c) : c(c), r(true)
{
}

void CheckMutable::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent->accept(*this);
    }

    if(r)
    {
        auto t = TypeVisitor::type(c, &node, false);

        if(t && t->constant)
        {
            r = false;
        }
    }
}

void CheckMutable::visit(CallNode &node)
{
    auto t = TypeVisitor::type(c, node.target.get());

    if(t->returnType && t->returnType->constant)
    {
        r = false;
    }
}

void CheckMutable::visit(ThisNode &node)
{
    if(TypeVisitor::type(c, &node)->constant)
    {
        r = false;
    }
}
