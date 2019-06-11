#ifndef PUSHVISITOR_H
#define PUSHVISITOR_H

#include "visitors/Visitor.h"

class Context;

class PushVisitor : public Visitor
{
public:
    PushVisitor(Context &c);

    virtual void visit(LiteralNode<char> &node) override;
    virtual void visit(LiteralNode<int> &node) override;
    virtual void visit(LiteralNode<std::size_t> &node) override;
    virtual void visit(IdNode &node) override;
    virtual void visit(AddrOfNode &node) override;

private:
    Context &c;
};

#endif // PUSHVISITOR_H
