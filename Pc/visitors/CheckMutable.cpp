#include "CheckMutable.h"

#include "nodes/IdNode.h"
#include "nodes/CallNode.h"

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
        if(TypeVisitor::type(c, &node)->constant)
        {
            r = false;
        }
    }
}

void CheckMutable::visit(CallNode &node)
{
    if(TypeVisitor::type(c, node.target.get())->constant)
    {
        r = false;
    }
}
