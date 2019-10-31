#include "TypeNode.h"

#include "visitors/Visitor.h"

void TypeNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string TypeNode::classname() const
{
    return "TypeNode";
}

Node *TypeNode::clone() const
{
    auto t = cloneNode(this, new TypeNode(location(), safeClone(name)));

    t->function = function;
    t->constant = constant;
    t->ref = ref;

    t->ptr = ptr;

    t->args = listClone(args);
    t->returnType = safeClone(returnType);

    return t;
}
