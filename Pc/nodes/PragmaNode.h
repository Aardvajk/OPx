#ifndef PRAGMANODE_H
#define PRAGMANODE_H

#include "application/Pragmas.h"

#include "nodes/Node.h"

class PragmaNode : public Node
{
public:
    PragmaNode(Location location, Pragmas::Type cmd) : Node(location), cmd(cmd) { }

    virtual void accept(Visitor &v) override;

    Pragmas::Type cmd;
    std::string arg;
};

#endif // PRAGMANODE_H
