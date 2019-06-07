#ifndef BYTELISTER_H
#define BYTELISTER_H

#include "visitors/Visitor.h"

#include <vector>

class ByteLister : public Visitor
{
public:
    ByteLister();

    std::vector<char> result() const { return v; }

    virtual void visit(IntLiteralNode &node) override;

private:
    std::vector<char> v;
};

#endif // BYTELISTER_H
