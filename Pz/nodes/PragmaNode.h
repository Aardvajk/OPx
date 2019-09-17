#ifndef PRAGMANODE_H
#define PRAGMANODE_H

#include "application/Pragmas.h"

#include "nodes/Node.h"

class PragmaNode : public Node
{
public:
    PragmaNode(Location location, Pragmas::Type type) : Node(location), type(type) { }

    virtual void accept(Visitor &v) override;
    virtual const char *classname() const override;

    Pragmas::Type type;
    std::string arg;
};

#endif // PRAGMANODE_H
