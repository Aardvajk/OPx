#ifndef THISCALLTRANSFORMER_H
#define THISCALLTRANSFORMER_H

#include "visitors/Visitor.h"

#include "nodes/Node.h"

class Context;

class ThisCallTransformer : public Visitor
{
public:
    explicit ThisCallTransformer(Context &c);

    NodePtr result(){ return rn; }

    virtual void visit(IdNode &node) override;

private:
    Context &c;
    NodePtr rn;
};

#endif // THISCALLTRANSFORMER_H
