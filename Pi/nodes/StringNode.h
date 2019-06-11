#ifndef STRINGNODE_H
#define STRINGNODE_H

#include "nodes/Node.h"

#include <string>

class StringNode : public Node
{
public:
    explicit StringNode(Location location, std::string value);

    virtual void accept(Visitor &visitor);

    std::string value;
};

#endif // STRINGNODE_H
