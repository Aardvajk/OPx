#ifndef EXPRTRANSFORMER_H
#define EXPRTRANSFORMER_H

#include "nodes/Node.h"

#include "visitors/Visitor.h"

class Context;

class ExprTransformer : public Visitor
{
public:
    explicit ExprTransformer(Context &c);

    NodePtr result(){ return rn; }

    virtual void visit(AssignNode &node) override;

    static NodePtr transform(Context &c, NodePtr &node);

private:
    Context &c;
    NodePtr rn;
};

#endif // EXPRTRANSFORMER_H
