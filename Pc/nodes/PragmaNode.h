#ifndef PRAGMANODE_H
#define PRAGMANODE_H

#include "application/Pragmas.h"

#include "nodes/Node.h"

class PragmaNode : public Node
{
public:
    PragmaNode(Location location, Pragmas::Type type, std::string arg = { }) : Node(location), type(type), arg(std::move(arg)) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    virtual Node *clone() const override;

    Pragmas::Type type;
    std::string arg;
};

#endif // PRAGMANODE_H
