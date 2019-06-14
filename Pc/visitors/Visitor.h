#ifndef VISITOR_H
#define VISITOR_H

class BlockNode;
class NamespaceNode;
class ClassNode;
class GlobalNode;
class IdNode;
class DotNode;
class VarNode;
class FuncNode;
class ScopeNode;
class IntLiteralNode;
class ExprNode;
class CallNode;

class Visitor
{
public:
    Visitor();
    virtual ~Visitor();

    virtual void visit(BlockNode &node);
    virtual void visit(NamespaceNode &node);
    virtual void visit(ClassNode &node);
    virtual void visit(GlobalNode &node);
    virtual void visit(IdNode &node);
    virtual void visit(DotNode &node);
    virtual void visit(VarNode &node);
    virtual void visit(FuncNode &node);
    virtual void visit(ScopeNode &node);
    virtual void visit(IntLiteralNode &node);
    virtual void visit(ExprNode &node);
    virtual void visit(CallNode &node);
};

#endif // VISITOR_H
