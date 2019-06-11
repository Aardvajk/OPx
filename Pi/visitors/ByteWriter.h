#ifndef BYTEWRITER_H
#define BYTEWRITER_H

#include "visitors/Visitor.h"

#include <vector>

class ByteWriter : public Visitor
{
public:
    ByteWriter(std::vector<char> &v);

    virtual void visit(LiteralNode<char> &node) override;
    virtual void visit(LiteralNode<int> &node) override;
    virtual void visit(LiteralNode<std::size_t> &node) override;
    virtual void visit(StringNode &node) override;

private:
    std::vector<char> &v;
};

#endif // BYTEWRITER_H
