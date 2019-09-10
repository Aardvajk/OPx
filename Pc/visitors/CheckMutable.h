#ifndef CHECKMUTABLE_H
#define CHECKMUTABLE_H

#include "visitors/Visitor.h"

class Context;

class CheckMutable : public Visitor
{
public:
    explicit CheckMutable(Context &c);

    bool result() const { return r; }

    virtual void visit(IdNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(ThisNode &node) override;

private:
    Context &c;
    bool r;
};

#endif // CHECKMUTABLE_H
