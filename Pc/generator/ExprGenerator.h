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

    pcx::optional<std::size_t> size() const { return sz; }

    virtual void visit(IdNode &node) override;
    virtual void visit(NullLiteralNode &node) override;
    virtual void visit(CharLiteralNode &node) override;
    virtual void visit(IntLiteralNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(AddrOfNode &node) override;

    static std::size_t generate(Context &c, std::ostream &os, Node &node);

private:
    Context &c;
    std::ostream &os;

    pcx::optional<std::size_t> sz;
};

#endif // EXPRGENERATOR_H
