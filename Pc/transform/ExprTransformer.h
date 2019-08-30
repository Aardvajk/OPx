#ifndef EXPRTRANSFORMER_H
#define EXPRTRANSFORMER_H

#include "nodes/Node.h"

#include "visitors/Visitor.h"

class Context;
class Type;

class ExprTransformer : public Visitor
{
public:
    explicit ExprTransformer(Context &c, const Type *expectedType);

    NodePtr result(){ return rn; }

    virtual void visit(IdNode &node) override;
    virtual void visit(StringLiteralNode &node) override;
    virtual void visit(AssignNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(AddrOfNode &node) override;
    virtual void visit(ThisNode &node) override;
    virtual void visit(DerefNode &node) override;
    virtual void visit(UnaryNode &node) override;
    virtual void visit(BinaryNode &node) override;
    virtual void visit(SubscriptNode &node) override;
    virtual void visit(LogicalNode &node) override;

    static NodePtr transform(Context &c, NodePtr &node, const Type *expectedType = nullptr);

private:
    Context &c;
    const Type *expectedType;

    NodePtr rn;
};

#endif // EXPRTRANSFORMER_H
