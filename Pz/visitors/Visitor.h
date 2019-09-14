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
