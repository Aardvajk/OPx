#ifndef EXPRLOWER_H
#define EXPRLOWER_H

#include "nodes/Node.h"

#include "visitors/Visitor.h"

#include <pcx/flags.h>

class Context;
class Type;

class ExprLower : public Visitor
{
public:
    enum class Flag
    {
        NoTopLevel = 1,
        NoThisDeref = 2
    };

    using Flags = pcx::flags<Flag>;

    explicit ExprLower(Context &c, NodePtr &cn, const Type *type, Flags flags);

    NodePtr result(){ return rn; }

    virtual void visit(BlockNode &node) override;
    virtual void visit(IdNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(AssignNode &node) override;
    virtual void visit(BinaryNode &node) override;
    virtual void visit(ThisNode &node) override;
    virtual void visit(AddrOfNode &node) override;
    virtual void visit(DerefNode &node) override;
    virtual void visit(IncDecNode &node) override;

    static NodePtr lower(Context &c, NodePtr &node, const Type *type = nullptr, Flags flags = { });

private:
    Context &c;
    NodePtr &cn;
    const Type *type;
    Flags flags;
    NodePtr rn;
};

template<> struct pcx_is_flag_enum<ExprLower::Flag> : std::true_type { };

#endif // EXPRLOWER_H
