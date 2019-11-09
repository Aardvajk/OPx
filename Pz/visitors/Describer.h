#ifndef DESCRIBER_H
#define DESCRIBER_H

#include "visitors/Visitor.h"

#include <string>

class Describer : public Visitor
{
public:
    Describer();

    std::string result() const { return r; }

    virtual void visit(BlockNode &node) override;
    virtual void visit(IdNode &node) override;
    virtual void visit(NamespaceNode &node) override;
    virtual void visit(TypeNode &node) override;
    virtual void visit(ClassNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(FuncNode &node) override;
    virtual void visit(ScopeNode &node) override;
    virtual void visit(IntLiteralNode &node) override;

private:
    std::string r;
};

#endif // DESCRIBER_H
