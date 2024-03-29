#ifndef DESCVISITOR_H
#define DESCVISITOR_H

#include "visitors/Visitor.h"

#include <string>

class DescVisitor : public Visitor
{
public:
    DescVisitor();

    std::string result() const { return r; }

    virtual void visit(BlockNode &node) override;
    virtual void visit(IdNode &node) override;
    virtual void visit(NamespaceNode &node) override;
    virtual void visit(TypeNode &node) override;
    virtual void visit(FuncNode &node) override;
    virtual void visit(ScopeNode &node) override;
    virtual void visit(ClassNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(CharLiteralNode &node) override;
    virtual void visit(IntLiteralNode &node) override;
    virtual void visit(SizeLiteralNode &node) override;
    virtual void visit(BoolLiteralNode &node) override;
    virtual void visit(StringLiteralNode &node) override;
    virtual void visit(ExprNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(ProxyCallNode &node) override;
    virtual void visit(ConstructNode &node) override;
    virtual void visit(PragmaNode &node) override;
    virtual void visit(AddrOfNode &node) override;
    virtual void visit(DerefNode &node) override;
    virtual void visit(ThisNode &node) override;
    virtual void visit(AssignNode &node) override;
    virtual void visit(UnaryNode &node) override;
    virtual void visit(BinaryNode &node) override;
    virtual void visit(LogicalNode &node) override;
    virtual void visit(InitNode &node) override;
    virtual void visit(PreIncDecNode &node) override;
    virtual void visit(PostIncDecNode &node) override;
    virtual void visit(TextNode &node) override;
    virtual void visit(CommaNode &node) override;
    virtual void visit(TernaryNode &node) override;
    virtual void visit(TypeCastNode &node) override;
    virtual void visit(SubscriptNode &node) override;
    virtual void visit(UncheckedCastNode &node) override;

private:
    std::string r;
};

#endif // DESCVISITOR_H
