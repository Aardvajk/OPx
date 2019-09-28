#ifndef EXPRTRANSFORM_H
#define EXPRTRANSFORM_H

#include "nodes/Node.h"

#include "visitors/Visitor.h"

class Context;
class Type;

class ExprTransform : public Visitor
{
public:
    ExprTransform(Context &c, Type *expectedType);

    NodePtr result(){ return rn; }

    virtual void visit(IdNode &node) override;
    virtual void visit(CharLiteralNode &node) override;
    virtual void visit(IntLiteralNode &node) override;
    virtual void visit(BoolLiteralNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(ConstructNode &node) override;
    virtual void visit(AddrOfNode &node) override;
    virtual void visit(DerefNode &node) override;
    virtual void visit(AssignNode &node) override;
    virtual void visit(BinaryNode &node) override;

    static NodePtr transform(Context &c, NodePtr &node, Type *expectedType = nullptr);

private:
    Context &c;
    Type *expectedType;

    NodePtr rn;
};

#endif // EXPRTRANSFORM_H
