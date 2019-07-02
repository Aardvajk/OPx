#ifndef TYPEVISITOR_H
#define TYPEVISITOR_H

#include "visitors/Visitor.h"

class Context;
class Type;

class TypeVisitor : public Visitor
{
public:
    explicit TypeVisitor(Context &c);

    const Type *result() const { return type; }

    virtual void visit(CharLiteralNode &node) override;
    virtual void visit(IntLiteralNode &node) override;

private:
    Context &c;
    const Type *type;
};

#endif // TYPEVISITOR_H
