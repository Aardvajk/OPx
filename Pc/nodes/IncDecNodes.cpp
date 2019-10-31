#include "IncDecNodes.h"

#include "visitors/Visitor.h"

#include <pcx/str.h>

void PreIncDecNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string PreIncDecNode::classname() const
{
    return pcx::str("pre", token.text());
}

Node *PreIncDecNode::clone() const
{
    return new PreIncDecNode(location(), token, safeClone(expr));
}

void PostIncDecNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string PostIncDecNode::classname() const
{
    return pcx::str("post", token.text());
}

Node *PostIncDecNode::clone() const
{
    return cloneNode(this, new PostIncDecNode(location(), token, safeClone(expr)));
}
