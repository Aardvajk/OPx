#ifndef FUNCTRANSFORM_H
#define FUNCTRANSFORM_H

#include "nodes/Node.h"

#include "visitors/Visitor.h"

#include <cstdlib>

class Context;

class FuncTransform : public Visitor
{
public:
    explicit FuncTransform(Context &c);

    NodePtr result(){ return rn; }

    virtual void visit(BlockNode &node) override;
    virtual void visit(ScopeNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(ExprNode &node) override;
    virtual void visit(ReturnNode &node) override;
    virtual void visit(InitNode &node) override;

    static NodePtr transform(Context &c, NodePtr &node);

private:
    Context &c;
    std::size_t index;

    NodePtr rn;
};

#endif // FUNCTRANSFORM_H
