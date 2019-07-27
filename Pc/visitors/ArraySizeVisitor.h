#ifndef ARRAYSIZEVISITOR_H
#define ARRAYSIZEVISITOR_H

#include "visitors/Visitor.h"

#include <pcx/optional.h>

class Node;

class ArraySizeVisitor : public Visitor
{
public:
    ArraySizeVisitor();

    pcx::optional<std::size_t> result() const { return r; }

    virtual void visit(IntLiteralNode &node) override;

    static std::size_t value(Node &node);

private:
    pcx::optional<std::size_t> r;
};

#endif // ARRAYSIZEVISITOR_H
