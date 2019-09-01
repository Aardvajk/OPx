#ifndef PRAGMANODE_H
#define PRAGMANODE_H

#include "nodes/Node.h"

class PragmaNode : public Node
{
public:
    PragmaNode(Location location, std::string cmd, std::string arg) : Node(location), cmd(std::move(cmd)), arg(std::move(arg)) { }

    virtual void accept(Visitor &v) override;

    std::string cmd;
    std::string arg;
};

#endif // PRAGMANODE_H
