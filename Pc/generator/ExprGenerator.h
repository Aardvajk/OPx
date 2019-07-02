#ifndef EXPRGENERATOR_H
#define EXPRGENERATOR_H

#include "visitors/Visitor.h"

#include <iostream>

class Context;

class ExprGenerator : public Visitor
{
public:
    ExprGenerator(Context &c, std::ostream &os);

    std::size_t size() const { return sz; }

    virtual void visit(CharLiteralNode &node) override;
    virtual void visit(IntLiteralNode &node) override;
    virtual void visit(CallNode &node) override;

private:
    Context &c;
    std::ostream &os;

    std::size_t sz;
};

#endif // EXPRGENERATOR_H
