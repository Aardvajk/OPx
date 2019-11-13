#ifndef TYPEBUILDER_H
#define TYPEBUILDER_H

#include "visitors/Visitor.h"

class Context;
class Type;

class TypeBuilder : public Visitor
{
public:
    explicit TypeBuilder(Context &c) : c(c), r(nullptr) { }

    Type *result() const { return r; }

    virtual void visit(TypeNode &node) override;

private:
    Context &c;
    Type *r;
};

#endif // TYPEBUILDER_H
