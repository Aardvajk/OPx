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
class BoolLiteralNode;
class StringLiteralNode;
class ExprNode;
class CallNode;
class ConstructNode;
class PragmaNode;
class AddrOfNode;
class DerefNode;
class ThisNode;
class AssignNode;
class BinaryNode;
class ReturnNode;

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
    virtual void visit(BoolLiteralNode &node);
    virtual void visit(StringLiteralNode &node);
    virtual void visit(ExprNode &node);
    virtual void visit(CallNode &node);
    virtual void visit(ConstructNode &node);
    virtual void visit(PragmaNode &node);
    virtual void visit(AddrOfNode &node);
    virtual void visit(DerefNode &node);
    virtual void visit(ThisNode &node);
    virtual void visit(AssignNode &node);
    virtual void visit(BinaryNode &node);
    virtual void visit(ReturnNode &node);

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
