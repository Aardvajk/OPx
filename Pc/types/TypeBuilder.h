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

    Type *result(){ return r; }

    virtual void visit(TypeNode &node) override;

    static Type *type(Context &c, Node *node);

private:
    Context &c;
    Type *r;
};

#endif // TYPEBUILDER_H
