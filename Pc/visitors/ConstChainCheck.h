#ifndef CONSTCHAINCHECK_H
#define CONSTCHAINCHECK_H

#include "visitors/Visitor.h"

class Context;

class ConstChainCheck : public Visitor
{
public:
    explicit ConstChainCheck(Context &c);

    bool result() const { return r; }

    virtual void visit(IdNode &node) override;
    virtual void visit(CallNode &node) override;

private:
    Context &c;
    bool r;
};

#endif // CONSTCHAINCHECK_H
