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
    virtual void visit(CharLiteralNode &node) override;
    virtual void visit(IntLiteralNode &node) override;
    virtual void visit(BoolLiteralNode &node) override;
    virtual void visit(StringLiteralNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(ConstructNode &node) override;
    virtual void visit(AddrOfNode &node) override;
    virtual void visit(DerefNode &node) override;
    virtual void visit(ThisNode &node) override;
    virtual void visit(AssignNode &node) override;
    virtual void visit(UnaryNode &node) override;
    virtual void visit(BinaryNode &node) override;
    virtual void visit(LogicalNode &node) override;
    virtual void visit(InitNode &node) override;
    virtual void visit(PreIncDecNode &node) override;
    virtual void visit(PostIncDecNode &node) override;
    virtual void visit(CommaNode &node) override;

    static Type *queryType(Context &c, Node *node);
    static Type *assertType(Context &c, Node *node);

private:
    Context &c;
    Type *r;
};

#endif // TYPEVISITOR_H
