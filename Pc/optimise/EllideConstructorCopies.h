#ifndef ELLIDECONSTRUCTORCOPIES_H
#define ELLIDECONSTRUCTORCOPIES_H

#include "visitors/Visitor.h"

class Context;
class VarNode;

class EllideConstructorCopies : public Visitor
{
public:
    EllideConstructorCopies(Context &c, VarNode &var);

    virtual void visit(CallNode &node) override;

private:
    Context &c;
    VarNode &var;
};

#endif // ELLIDECONSTRUCTORCOPIES_H
