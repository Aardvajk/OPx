#ifndef DESCRIBER_H
#define DESCRIBER_H

#include "visitors/Visitor.h"

#include <string>

class Describer : public Visitor
{
public:
    Describer();

    std::string result() const { return r; }

    virtual void visit(BlockNode &node) override;
    virtual void visit(IdNode &node) override;
    virtual void visit(NamespaceNode &node) override;
    virtual void visit(FuncNode &node) override;

private:
    std::string r;
};

#endif // DESCRIBER_H
