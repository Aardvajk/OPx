#ifndef TEXTNODE_H
#define TEXTNODE_H

#include "nodes/Node.h"

class TextNode : public Node
{
public:
    TextNode(Location location, std::string value) : Node(location), value(std::move(value)) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    std::string value;
};

#endif // TEXTNODE_H
