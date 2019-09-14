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

private:
    std::string r;
};

#endif // DESCVISITOR_H
