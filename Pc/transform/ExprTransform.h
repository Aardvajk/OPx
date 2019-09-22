#ifndef EXPRTRANSFORM_H
#define EXPRTRANSFORM_H

#include "nodes/Node.h"

#include "visitors/Visitor.h"

class Context;

class ExprTransform : public Visitor
{
public:
    explicit ExprTransform(Context &c);

    NodePtr result(){ return rn; }

    virtual void visit(IdNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(AddrOfNode &node) override;
    virtual void visit(DerefNode &node) override;
    virtual void visit(AssignNode &node) override;
    virtual void visit(BinaryNode &node) override;

    static NodePtr transform(Context &c, NodePtr &node);

private:
    Context &c;
    NodePtr rn;
};

#endif // EXPRTRANSFORM_H
