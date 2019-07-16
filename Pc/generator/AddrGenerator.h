#ifndef ADDRGENERATOR_H
#define ADDRGENERATOR_H

#include "visitors/Visitor.h"

#include <iostream>

class Context;
class Node;

class AddrGenerator : public Visitor
{
public:
    AddrGenerator(Context &c, std::ostream &os);

    bool result() const { return ok; }

    virtual void visit(IdNode &node) override;

private:
    Context &c;
    std::ostream &os;

    bool ok;
};

#endif // ADDRGENERATOR_H
