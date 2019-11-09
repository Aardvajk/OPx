#ifndef VISITOR_H
#define VISITOR_H

#include <utility>

class BlockNode;

class Visitor
{
public:
    virtual ~Visitor();

    virtual void visit(BlockNode &node);

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
