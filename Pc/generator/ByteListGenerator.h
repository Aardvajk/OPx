#ifndef BYTELISTGENERATOR_H
#define BYTELISTGENERATOR_H

#include "visitors/Visitor.h"

#include <iostream>

class Context;

class ByteListGenerator : public Visitor
{
public:
    ByteListGenerator(Context &c, std::ostream &os);

    bool result() const { return ok; }

    virtual void visit(CharLiteralNode &node) override;
    virtual void visit(IntLiteralNode &node) override;
    virtual void visit(BoolLiteralNode &node) override;

private:
    Context &c;
    std::ostream &os;

    bool ok;
};

#endif // BYTELISTGENERATOR_H
