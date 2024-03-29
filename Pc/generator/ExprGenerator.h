#ifndef EXPRGENERATOR_H
#define EXPRGENERATOR_H

#include "visitors/Visitor.h"

#include <pcx/optional.h>

#include <iostream>

class Context;
class Node;

class ExprGenerator : public Visitor
{
public:
    ExprGenerator(Context &c, std::ostream &os);

    pcx::optional<std::size_t> result() const { return sz; }

    virtual void visit(IdNode &node) override;
    virtual void visit(CharLiteralNode &node) override;
    virtual void visit(IntLiteralNode &node) override;
    virtual void visit(SizeLiteralNode &node) override;
    virtual void visit(BoolLiteralNode &node) override;
    virtual void visit(StringLiteralNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(ProxyCallNode &node) override;
    virtual void visit(ConstructNode &node) override;
    virtual void visit(AddrOfNode &node) override;
    virtual void visit(DerefNode &node) override;
    virtual void visit(ThisNode &node) override;
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

    static std::size_t generate(Context &c, std::ostream &os, Node *node);

private:
    Context &c;
    std::ostream &os;

    pcx::optional<std::size_t> sz;
};

#endif // EXPRGENERATOR_H
