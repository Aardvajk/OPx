#ifndef EXPRCONVERT_H
#define EXPRCONVERT_H

#include "nodes/Node.h"

#include "visitors/Visitor.h"

class Context;
class Type;

class ExprConvert : public Visitor
{
public:
    ExprConvert(Context &c, NodePtr &cn, Type *expectedType);

    NodePtr result(){ return rn; }

    virtual void visit(CallNode &node) override;
    virtual void visit(ProxyCallNode &node) override;

    static NodePtr convert(Context &c, NodePtr &node, Type *expectedType = nullptr);

private:
    Context &c;
    NodePtr &cn;
    Type *expectedType;

    NodePtr rn;
};

#endif // EXPRCONVERT_H
