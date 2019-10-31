#include "ClassNode.h"

#include "visitors/Visitor.h"

void ClassNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string ClassNode::classname() const
{
    return "class";
}

Node *ClassNode::clone() const
{
    return new ClassNode(location(), safeClone(name), safeClone(body));
}
