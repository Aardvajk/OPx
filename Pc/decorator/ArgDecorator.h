#ifndef ARGDECORATOR_H
#define ARGDECORATOR_H

#include "visitors/Visitor.h"

class Context;

class ArgDecorator : public Visitor
{
public:
    ArgDecorator(Context &c, std::size_t &no);

    virtual void visit(VarNode &node) override;

private:
    Context &c;
    std::size_t &no;
};

#endif // ARGDECORATOR_H
