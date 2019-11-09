#include "FuncNode.h"

#include "visitors/Visitor.h"

void FuncNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string FuncNode::classname() const
{
    return "func";
}

NodePtr FuncNode::cloneDetail() const
{
    auto f = copyDetails(this, new FuncNode(location(), clonePtr(name)));
    NodePtr n(f);

    f->genericTags = cloneList(genericTags);
    f->args = cloneList(args);
    f->type = clonePtr(type);
    f->body = clonePtr(body);

    return n;
}
