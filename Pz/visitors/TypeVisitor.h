#ifndef TYPEVISITOR_H
#define TYPEVISITOR_H

#include "visitors/Visitor.h"

class Type;

class TypeVisitor : public Visitor
{
public:
    TypeVisitor();

    Type *result() const { return r; }

    virtual void visit(IdNode &node) override;
    virtual void visit(TypeNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(IntLiteralNode &node) override;
    virtual void visit(CallNode &node) override;

private:
    Type *r;
};

#endif // TYPEVISITOR_H
