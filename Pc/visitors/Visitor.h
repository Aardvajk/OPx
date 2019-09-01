#ifndef VISITOR_H
#define VISITOR_H

class BlockNode;
class IdNode;
class NamespaceNode;
class ClassNode;
class VarNode;
class FuncNode;
class ScopeNode;
class TypeNode;
class NullLiteralNode;
class CharLiteralNode;
class IntLiteralNode;
class BoolLiteralNode;
class SizeLiteralNode;
class StringLiteralNode;
class ExprNode;
class CallNode;
class ReturnNode;
class AddrOfNode;
class AssignNode;
class ThisNode;
class DerefNode;
class UnaryNode;
class BinaryNode;
class SubscriptNode;
class WhileNode;
class PrimitiveCastNode;
class LogicalNode;
class IfNode;
class InitNode;
class ForNode;

class Visitor
{
public:
    virtual ~Visitor();

    virtual void visit(BlockNode &node);
    virtual void visit(IdNode &node);
    virtual void visit(NamespaceNode &node);
    virtual void visit(ClassNode &node);
    virtual void visit(VarNode &node);
    virtual void visit(FuncNode &node);
    virtual void visit(ScopeNode &node);
    virtual void visit(TypeNode &node);
    virtual void visit(NullLiteralNode &node);
    virtual void visit(CharLiteralNode &node);
    virtual void visit(IntLiteralNode &node);
    virtual void visit(BoolLiteralNode &node);
    virtual void visit(SizeLiteralNode &node);
    virtual void visit(StringLiteralNode &node);
    virtual void visit(ExprNode &node);
    virtual void visit(CallNode &node);
    virtual void visit(ReturnNode &node);
    virtual void visit(AddrOfNode &node);
    virtual void visit(AssignNode &node);
    virtual void visit(ThisNode &node);
    virtual void visit(DerefNode &node);
    virtual void visit(UnaryNode &node);
    virtual void visit(BinaryNode &node);
    virtual void visit(SubscriptNode &node);
    virtual void visit(WhileNode &node);
    virtual void visit(PrimitiveCastNode &node);
    virtual void visit(LogicalNode &node);
    virtual void visit(IfNode &node);
    virtual void visit(InitNode &node);
    virtual void visit(ForNode &node);
};

#endif // VISITOR_H
