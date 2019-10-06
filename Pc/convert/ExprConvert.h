#ifndef EXPRCONVERT_H
#define EXPRCONVERT_H

#include "nodes/Node.h"

#include "visitors/Visitor.h"

class Context;
class Type;

class ExprConvert : public Visitor
{
public:
    ExprConvert(Context &c, Type *expectedType);

    NodePtr result(){ return rn; }

    static NodePtr convert(Context &c, NodePtr &node, Type *expectedType = nullptr);

private:
    Context &c;
    Type *expectedType;

    NodePtr rn;
};

#endif // EXPRCONVERT_H
