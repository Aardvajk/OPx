#ifndef DESCVISITOR_H
#define DESCVISITOR_H

#include "visitors/Visitor.h"

#include <string>

class Node;

class DescVisitor : public Visitor
{
public:
    DescVisitor();

    std::string result() const { return r; }

    virtual void visit(BlockNode &node) override;

private:
    std::string r;
};

#endif // DESCVISITOR_H
