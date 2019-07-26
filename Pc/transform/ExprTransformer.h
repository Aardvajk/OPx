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

    virtual void visit(IdNode &node) override;
    virtual void visit(AssignNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(AddrOfNode &node) override;
    virtual void visit(ThisNode &node) override;
    virtual void visit(DerefNode &node) override;

    static NodePtr transform(Context &c, NodePtr &node);
    static void transform(Context &c, NodeList &nodes);

private:
    Context &c;
    NodePtr rn;
};

#endif // EXPRTRANSFORMER_H
