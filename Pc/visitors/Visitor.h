#ifndef VISITOR_H
#define VISITOR_H

class BlockNode;
class NamespaceNode;
class ClassNode;
class GlobalNode;
class IdNode;
class DotNode;

class Visitor
{
public:
    Visitor();

    virtual void visit(BlockNode &node);
    virtual void visit(NamespaceNode &node);
    virtual void visit(ClassNode &node);
    virtual void visit(GlobalNode &node);
    virtual void visit(IdNode &node);
    virtual void visit(DotNode &node);
};

#endif // VISITOR_H
