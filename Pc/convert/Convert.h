#ifndef CONVERT_H
#define CONVERT_H

#include "visitors/Visitor.h"

class Context;

class Convert : public Visitor
{
public:
    explicit Convert(Context &c);

private:
    Context &c;

    // Visitor interface
public:
    virtual void visit(BlockNode &node) override;
    virtual void visit(NamespaceNode &node) override;
    virtual void visit(FuncNode &node) override;
    virtual void visit(ClassNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(PragmaNode &node) override;
};

#endif // CONVERT_H
