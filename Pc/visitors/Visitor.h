#ifndef VISITOR_H
#define VISITOR_H

class BlockNode;
class IdNode;
class DotNode;
class NamespaceNode;
class ClassNode;
class VarNode;
class FuncNode;
class TypeNode;
class IntLiteralNode;
class ExprNode;
class CallNode;

class Visitor
{
public:
    virtual ~Visitor();

    virtual void visit(BlockNode &node);
    virtual void visit(IdNode &node);
    virtual void visit(DotNode &node);
    virtual void visit(NamespaceNode &node);
    virtual void visit(ClassNode &node);
    virtual void visit(VarNode &node);
    virtual void visit(FuncNode &node);
    virtual void visit(TypeNode &node);
    virtual void visit(IntLiteralNode &node);
    virtual void visit(ExprNode &node);
    virtual void visit(CallNode &node);
};

#endif // VISITOR_H
