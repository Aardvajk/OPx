#ifndef NAMEVISITORS_H
#define NAMEVISITORS_H

#include "visitors/Visitor.h"

#include <string>

class Node;

namespace NameVisitors
{

class PrettyName : public Visitor
{
public:
    PrettyName();

    std::string result() const { return r; }

    virtual void visit(IdNode &node) override;
    virtual void visit(DotNode &node) override;

private:
    std::string r;
};

std::string prettyName(Node *node);

}

#endif // NAMEVISITORS_H
