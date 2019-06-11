#ifndef ADDRVISITOR_H
#define ADDRVISITOR_H

#include "visitors/Visitor.h"

class Context;

class AddrVisitor : public Visitor
{
public:
    AddrVisitor(Context &c);

    virtual void visit(IdNode &node) override;

private:
    Context &c;
};

#endif // ADDRVISITOR_H
