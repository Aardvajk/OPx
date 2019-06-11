#ifndef ASTPRINTER_H
#define ASTPRINTER_H

#include "visitors/Visitor.h"

#include <iostream>
#include <string>

class Node;

class AstPrinter : public Visitor
{
public:
    AstPrinter(std::ostream &os);

    virtual void visit(LiteralNode<char> &node) override;
    virtual void visit(LiteralNode<int> &node) override;
    virtual void visit(LiteralNode<std::size_t> &node) override;
    virtual void visit(StringNode &node) override;
    virtual void visit(IdNode &node) override;
    virtual void visit(AddrOfNode &node) override;

private:
    std::ostream &os;
};

std::string astReconstruct(Node *node);

#endif // ASTPRINTER_H
