#ifndef INITMAPBUILDER_H
#define INITMAPBUILDER_H

#include "visitors/Visitor.h"

#include "nodes/Node.h"

#include <unordered_map>

class Node;

class InitMapBuilder : public Visitor
{
public:
    InitMapBuilder(std::unordered_map<std::string, NodePtr> &m, NodePtr &n);

    virtual void visit(InitNode &node) override;

private:
    std::unordered_map<std::string, NodePtr> &m;
    NodePtr &n;
};

#endif // INITMAPBUILDER_H
