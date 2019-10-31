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

Node *FuncNode::clone() const
{
    auto f = cloneNode(this, new FuncNode(location(), name->clone()));

    f->args = listClone(args);
    f->inits = listClone(inits);
    f->type = safeClone(type);
    f->body = safeClone(body);
    f->constMethod = constMethod;

    return f;
}


