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
    virtual void visit(SizeLiteralNode &node) override;
    virtual void visit(BoolLiteralNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(ProxyCallNode &node) override;
    virtual void visit(ConstructNode &node) override;
    virtual void visit(AddrOfNode &node) override;
    virtual void visit(DerefNode &node) override;
    virtual void visit(AssignNode &node) override;
    virtual void visit(UnaryNode &node) override;
    virtual void visit(BinaryNode &node) override;
    virtual void visit(LogicalNode &node) override;
    virtual void visit(PreIncDecNode &node) override;
    virtual void visit(PostIncDecNode &node) override;
    virtual void visit(CommaNode &node) override;
    virtual void visit(InlineVarNode &node) override;
    virtual void visit(TernaryNode &node) override;
    virtual void visit(TypeCastNode &node) override;

    static NodePtr transform(Context &c, NodePtr &node, Type *expectedType = nullptr);

private:
    Context &c;
    Type *expectedType;

    NodePtr rn;
};

#endif // EXPRTRANSFORM_H
