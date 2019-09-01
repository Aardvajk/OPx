#include "EllideConstructorCopies.h"

#include "application/Context.h"

#include "nodes/VarNode.h"
#include "nodes/CallNode.h"

#include "types/Type.h"

#include "visitors/TypeVisitor.h"

EllideConstructorCopies::EllideConstructorCopies(Context &c, VarNode &var) : c(c), var(var)
{
}

void EllideConstructorCopies::visit(CallNode &node)
{
    auto t = TypeVisitor::type(c, node.target.get());

    if(!t->function())
    {
        for(auto &p: node.params)
        {
            var.params.push_back(p);
        }

        var.value = { };
    }
}
