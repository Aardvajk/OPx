#ifndef IDRESOLVER_H
#define IDRESOLVER_H

#include "visitors/Visitor.h"

#include <string>

class IdResolver : public Visitor
{
public:
    IdResolver();

    std::string result() const { return s; }

    virtual void visit(IdNode &node) override;
    virtual void visit(DotNode &node) override;

private:
    std::string s;
};

#endif // IDRESOLVER_H
