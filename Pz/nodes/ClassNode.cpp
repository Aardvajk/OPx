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

NodePtr ClassNode::cloneDetail() const
{
    auto c = copyDetails(this, new ClassNode(location(), clonePtr(name)));
    NodePtr n(c);

    c->genericTags = cloneList(genericTags);
    c->body = clonePtr(body);

    return n;
}
