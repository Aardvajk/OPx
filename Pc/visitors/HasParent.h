#ifndef HASPARENT_H
#define HASPARENT_H

#include "visitors/Visitor.h"

class HasParent : public Visitor
{
public:
    HasParent();

    bool result() const { return r; }

    virtual void visit(IdNode &node) override;

private:
    bool r;
};

#endif // HASPARENT_H
