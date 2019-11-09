#include "GenericTagNode.h"

#include "visitors/Visitor.h"

void GenericTagNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string GenericTagNode::classname() const
{
    return "generictag";
}

NodePtr GenericTagNode::cloneDetail() const
{
    return copyDetails(this, new GenericTagNode(location(), name));
}
