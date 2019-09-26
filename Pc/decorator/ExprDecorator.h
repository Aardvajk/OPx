#ifndef EXPRDECORATOR_H
#define EXPRDECORATOR_H

#include "nodes/Node.h"

#include "visitors/Visitor.h"

#include <pcx/flags.h>

class Context;
class Type;

class ExprDecorator : public Visitor
{
public:
    enum class Flag
    {
        SkipParams = 1
    };

    using Flags = pcx::flags<Flag>;

    explicit ExprDecorator(Context &c, Type *expectedType, Flags flags);

    NodePtr result(){ return rn; }

    virtual void visit(IdNode &node) override;
    virtual void visit(StringLiteralNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(AddrOfNode &node) override;
    virtual void visit(DerefNode &node) override;
    virtual void visit(ThisNode &node) override;
    virtual void visit(AssignNode &node) override;
    virtual void visit(BinaryNode &node) override;

    static NodePtr decorate(Context &c, NodePtr node, Type *expectedType = nullptr, Flags flags = { });

private:
    Context &c;
    Type *expectedType;
    Flags flags;

    NodePtr rn;
};

template<> struct pcx_is_flag_enum<ExprDecorator::Flag> : std::true_type { };

#endif // EXPRDECORATOR_H
