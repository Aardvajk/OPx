#ifndef TYPEVISITOR_H
#define TYPEVISITOR_H

#include "visitors/Visitor.h"

class Context;
class Type;
class Node;

class TypeVisitor : public Visitor
{
public:
    explicit TypeVisitor(Context &c);

    Type *result(){ return r; }

    virtual void visit(IdNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(NullLiteralNode &node) override;
    virtual void visit(CharLiteralNode &node) override;
    virtual void visit(IntLiteralNode &node) override;
    virtual void visit(BoolLiteralNode &node) override;
    virtual void visit(SizeLiteralNode &node) override;
    virtual void visit(StringLiteralNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(AddrOfNode &node) override;
    virtual void visit(AssignNode &node) override;
    virtual void visit(ThisNode &node) override;
    virtual void visit(DerefNode &node) override;
    virtual void visit(UnaryNode &node) override;
    virtual void visit(BinaryNode &node) override;
    virtual void visit(SubscriptNode &node) override;
    virtual void visit(PrimitiveCastNode &node) override;
    virtual void visit(LogicalNode &node) override;

    static Type *type(Context &c, Node *node);

private:
    Context &c;
    Type *r;
};

#endif // TYPEVISITOR_H
