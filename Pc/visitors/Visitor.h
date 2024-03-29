#ifndef VISITOR_H
#define VISITOR_H

#include <utility>

class BlockNode;
class IdNode;
class NamespaceNode;
class TypeNode;
class FuncNode;
class ScopeNode;
class ClassNode;
class VarNode;
class CharLiteralNode;
class IntLiteralNode;
class SizeLiteralNode;
class BoolLiteralNode;
class StringLiteralNode;
class ExprNode;
class CallNode;
class ProxyCallNode;
class ConstructNode;
class PragmaNode;
class AddrOfNode;
class DerefNode;
class ThisNode;
class AssignNode;
class UnaryNode;
class BinaryNode;
class LogicalNode;
class ReturnNode;
class InitNode;
class PreIncDecNode;
class PostIncDecNode;
class WhileNode;
class IfNode;
class ForNode;
class TextNode;
class CommaNode;
class InlineVarNode;
class TernaryNode;
class TypeCastNode;
class SubscriptNode;
class UncheckedCastNode;

class Visitor
{
public:
    virtual ~Visitor();

    virtual void visit(BlockNode &node);
    virtual void visit(IdNode &node);
    virtual void visit(NamespaceNode &node);
    virtual void visit(TypeNode &node);
    virtual void visit(FuncNode &node);
    virtual void visit(ScopeNode &node);
    virtual void visit(ClassNode &node);
    virtual void visit(VarNode &node);
    virtual void visit(CharLiteralNode &node);
    virtual void visit(IntLiteralNode &node);
    virtual void visit(SizeLiteralNode &node);
    virtual void visit(BoolLiteralNode &node);
    virtual void visit(StringLiteralNode &node);
    virtual void visit(ExprNode &node);
    virtual void visit(CallNode &node);
    virtual void visit(ProxyCallNode &node);
    virtual void visit(ConstructNode &node);
    virtual void visit(PragmaNode &node);
    virtual void visit(AddrOfNode &node);
    virtual void visit(DerefNode &node);
    virtual void visit(ThisNode &node);
    virtual void visit(AssignNode &node);
    virtual void visit(UnaryNode &node);
    virtual void visit(BinaryNode &node);
    virtual void visit(LogicalNode &node);
    virtual void visit(ReturnNode &node);
    virtual void visit(InitNode &node);
    virtual void visit(PreIncDecNode &node);
    virtual void visit(PostIncDecNode &node);
    virtual void visit(WhileNode &node);
    virtual void visit(IfNode &node);
    virtual void visit(ForNode &node);
    virtual void visit(TextNode &node);
    virtual void visit(CommaNode &node);
    virtual void visit(InlineVarNode &node);
    virtual void visit(TernaryNode &node);
    virtual void visit(TypeCastNode &node);
    virtual void visit(SubscriptNode &node);
    virtual void visit(UncheckedCastNode &node);

    template<typename T, typename N, typename... Args> static void visit(N node, Args&&... args)
    {
        T v(std::forward<Args>(args)...);
        node->accept(v);
    }

    template<typename T, typename R, typename N, typename... Args> static R query(N node, Args&&... args)
    {
        T v(std::forward<Args>(args)...);
        node->accept(v);

        return v.result();
    }
};

#endif // VISITOR_H
