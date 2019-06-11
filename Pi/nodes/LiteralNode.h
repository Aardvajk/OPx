#ifndef LITERALNODE_H
#define LITERALNODE_H

#include "nodes/Node.h"

#include "visitors/Visitor.h"

template<typename T> class LiteralNode : public Node
{
public:
    explicit LiteralNode(Location location, const T &value) : Node(location), value(value) { }

    virtual void accept(Visitor &visitor);

    T value;
};

template<typename T> void LiteralNode<T>::accept(Visitor &visitor)
{
    visitor.visit(*this);
}

#endif // LITERALNODE_H
