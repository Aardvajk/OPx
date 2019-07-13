#ifndef TYPEBUILDER_H
#define TYPEBUILDER_H

#include "visitors/Visitor.h"

class Context;
class Type;
class Node;

class TypeBuilder : public Visitor
{
public:
    TypeBuilder(Context &c);

    const Type *result() const { return r; }

    virtual void visit(VarNode &node) override;
    virtual void visit(TypeNode &node) override;

    static const Type *type(Context &c, Node *node);

private:
    Context &c;
    const Type *r;
};

#endif // TYPEBUILDER_H
