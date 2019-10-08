#ifndef BYTELISTGENERATOR_H
#define BYTELISTGENERATOR_H

#include "visitors/Visitor.h"

#include <iostream>

class Context;

class CanByteListGenerate : public Visitor
{
public:
    CanByteListGenerate();

    bool result() const { return ok; }

    virtual void visit(CharLiteralNode &node) override;
    virtual void visit(IntLiteralNode &node) override;
    virtual void visit(BoolLiteralNode &node) override;
    virtual void visit(StringLiteralNode &node) override;

private:
    bool ok;
};

class ByteListGenerator : public Visitor
{
public:
    ByteListGenerator(Context &c, std::ostream &os);

    virtual void visit(CharLiteralNode &node) override;
    virtual void visit(IntLiteralNode &node) override;
    virtual void visit(BoolLiteralNode &node) override;
    virtual void visit(StringLiteralNode &node) override;

private:
    Context &c;
    std::ostream &os;
};

#endif // BYTELISTGENERATOR_H
