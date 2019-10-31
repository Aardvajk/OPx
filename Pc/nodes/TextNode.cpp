#include "TextNode.h"

#include "visitors/Visitor.h"

void TextNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string TextNode::classname() const
{
    return "text";
}

Node *TextNode::clone() const
{
    return cloneNode(this, new TextNode(location(), value));
}
