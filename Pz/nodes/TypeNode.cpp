#include "TypeNode.h"

#include "visitors/Visitor.h"

void TypeNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string TypeNode::classname() const
{
    return "type";
}

NodePtr TypeNode::cloneDetail() const
{
    auto t = copyDetails(this, new TypeNode(location(), clonePtr(name)));
    NodePtr n(t);

    t->function = function;
    t->constant = constant;
    t->ref = ref;

    t->ptr = ptr;

    t->args = cloneList(args);
    t->returnType = clonePtr(returnType);

    return n;
}
