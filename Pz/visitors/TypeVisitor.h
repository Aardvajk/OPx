#ifndef TYPEVISITOR_H
#define TYPEVISITOR_H

#include "visitors/Visitor.h"

class Context;
class Type;
class Node;

class TypeVisitor : public Visitor
{
public:
    TypeVisitor(Context &c);

    Type *result() const { return r; }

    virtual void visit(IdNode &node) override;
    virtual void visit(TypeNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(IntLiteralNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(ConstructNode &node) override;

    static Type *queryType(Context &c, Node *node);
    static Type *assertType(Context &c, Node *node);

private:
    Context &c;
    Type *r;
};

#endif // TYPEVISITOR_H
