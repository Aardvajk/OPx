#ifndef IDNODE_H
#define IDNODE_H

#include "nodes/Node.h"

#include <string>

class IdNode : public Node
{
public:
    explicit IdNode(Location location, std::string value);

    virtual void accept(Visitor &visitor);

    std::string value;
};

#endif // IDNODE_H
