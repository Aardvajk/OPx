#ifndef EXPRLOWER_H
#define EXPRLOWER_H

#include "nodes/Node.h"

#include "visitors/Visitor.h"

#include <pcx/flags.h>

class Context;

class ExprLower : public Visitor
{
public:
    enum class Flag
    {
        NoTopLevel = 1
    };

    using Flags = pcx::flags<Flag>;

    explicit ExprLower(Context &c, NodePtr &cn, Flags flags);

    NodePtr result(){ return rn; }

    virtual void visit(IdNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(AssignNode &node) override;
    virtual void visit(BinaryNode &node) override;

    static NodePtr lower(Context &c, NodePtr &node, Flags flags = { });
    static void lower(Context &c, NodeList &nodes, Flags flags = { });

private:
    Context &c;
    NodePtr &cn;
    Flags flags;
    NodePtr rn;
};

template<> struct pcx_is_flag_enum<ExprLower::Flag> : std::true_type { };

#endif // EXPRLOWER_H
