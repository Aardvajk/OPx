#ifndef VISITOR_H
#define VISITOR_H

class BlockNode;

class Visitor
{
public:
    virtual ~Visitor();

    virtual void visit(BlockNode &node);
};

#endif // VISITOR_H
