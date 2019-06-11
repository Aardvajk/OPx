#ifndef VISITOR_H
#define VISITOR_H

#include <cstring>

template<typename T> class LiteralNode;
class StringNode;
class IdNode;
class AddrOfNode;

class Visitor
{
public:
    Visitor() : ok(false) { }
    virtual ~Visitor();

    bool okay() const { return ok; }

    virtual void visit(LiteralNode<char> &node);
    virtual void visit(LiteralNode<int> &node);
    virtual void visit(LiteralNode<std::size_t> &node);
    virtual void visit(StringNode &node);
    virtual void visit(IdNode &node);
    virtual void visit(AddrOfNode &node);

protected:
    bool ok;
};

#endif // VISITOR_H
