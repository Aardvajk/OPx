#ifndef EXPRLOWER_H
#define EXPRLOWER_H

#include "nodes/Node.h"

#include "visitors/Visitor.h"

class Context;

class ExprLower : public Visitor
{
public:
    explicit ExprLower(Context &c);

    NodePtr result(){ return rn; }

    virtual void visit(AssignNode &node) override;

    static NodePtr lower(Context &c, NodePtr &node);

private:
    Context &c;
    NodePtr rn;
};

#endif // EXPRLOWER_H
