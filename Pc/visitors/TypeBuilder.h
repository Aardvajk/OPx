#ifndef TYPEBUILDER_H
#define TYPEBUILDER_H

#include "visitors/Visitor.h"

#include <pcx/scoped_ptr.h>

class Context;
class Type;

class TypeBuilder : public Visitor
{
public:
    TypeBuilder(Context &c);
    virtual ~TypeBuilder() override;

    Type *claim();

    virtual void visit(TypeNode &node) override;

private:
    Context &c;
    pcx::scoped_ptr<Type> t;
};

#endif // TYPEBUILDER_H
