#ifndef EXPRLOWER_H
#define EXPRLOWER_H

#include "nodes/Node.h"

#include "visitors/Visitor.h"

class Context;
class Type;

class ExprLower : public Visitor
{
public:
    explicit ExprLower(Context &c, NodePtr &cn, Type *expectedType);

    NodePtr result(){ return rn; }

    virtual void visit(IdNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(ConstructNode &node) override;
    virtual void visit(AddrOfNode &node) override;
    virtual void visit(DerefNode &node) override;
    virtual void visit(AssignNode &node) override;
    virtual void visit(BinaryNode &node) override;

    static NodePtr lower(Context &c, NodePtr node, Type *expectedType = nullptr);

private:
    Context &c;
    NodePtr &cn;
    Type *expectedType;

    NodePtr rn;
};

#endif // EXPRLOWER_H
