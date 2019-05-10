#ifndef VISITOR_H
#define VISITOR_H

class BlockNode;
class IdNode;
class DotNode;
class ClassNode;

class Visitor
{
public:
    Visitor();

    virtual void visit(BlockNode &node);
    virtual void visit(IdNode &node);
    virtual void visit(DotNode &node);
    virtual void visit(ClassNode &node);
};

#endif // VISITOR_H
