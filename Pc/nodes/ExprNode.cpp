#include "ExprNode.h"

#include "visitors/Visitor.h"

void ExprNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string ExprNode::classname() const
{
    return "expr";
}

Node *ExprNode::clone() const
{
    return new ExprNode(location(), safeClone(expr));
}
