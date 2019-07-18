#ifndef BLOCKNODE_H
#define BLOCKNODE_H

#include "nodes/Node.h"

class BlockNode : public Node
{
public:
    explicit BlockNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;

    void push_back(Node *node);
    void insert(std::size_t index, Node *node);

    NodeList nodes;
};

#endif // BLOCKNODE_H
