#include "OperatorCallDecorate.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/ProxyCallNode.h"

#include "types/Type.h"

#include "decorator/CommonDecorator.h"
#include "decorator/ExprDecorator.h"

#include "visitors/TypeVisitor.h"

namespace
{

NodePtr generateMethodOperatorCall(Context &c, Node &node, NodePtr first, NodeList &params, const std::string &op, Sym *sym)
{
    auto cn = new ProxyCallNode(node.location(), sym, first);
    NodePtr rn = cn;

    for(auto &p: params)
    {
        cn->params.push_back(p);
    }

    rn = ExprDecorator::decorate(c, rn);
    return rn;
}

NodePtr generateFreeOperatorCall(Context &c, Node &node, NodePtr first, NodeList &params, const std::string &op, Sym *sym)
{
    auto cn = new ProxyCallNode(node.location(), sym);
    NodePtr rn = cn;

    cn->params.push_back(first);
    for(auto &p: params)
    {
        cn->params.push_back(p);
    }

    rn = ExprDecorator::decorate(c, rn);
    return rn;
}

std::vector<Sym*> searchMethod(Context &c, Node &node, NodePtr first, NodeList &params, const std::string &op)
{
    NodePtr id(new IdNode(node.location(), first, pcx::str("operator", op)));

    auto t = Type::makeFunction(c.types.nullType());
    for(auto &p: params)
    {
        t.args.push_back(TypeVisitor::assertType(c, p.get()));
    }

    return CommonDecorator::searchCallable(c, id.get(), &t);
}

std::vector<Sym*> searchFree(Context &c, Node &node, NodePtr first, NodeList &params, const std::string &op)
{
    NodePtr id(new IdNode(node.location(), { }, pcx::str("operator", op)));

    auto t = Type::makeFunction(c.types.nullType(), { TypeVisitor::assertType(c, first.get()) });
    for(auto &p: params)
    {
        t.args.push_back(TypeVisitor::assertType(c, p.get()));
    }

    return CommonDecorator::searchCallable(c, id.get(), &t);
}

}

NodePtr OperatorCallDecorate::generate(Context &c, Node &node, NodePtr first, NodeList &params, const std::string &op)
{
    auto ms = searchMethod(c, node, first, params, op);
    auto fs = searchFree(c, node, first, params, op);

    if(ms.empty() && fs.empty())
    {
        throw Error(node.location(), "not found - operator", op, "()");
    }

    if(!ms.empty() && !fs.empty())
    {
        throw Error(node.location(), "ambiguous - operator", op, "()");
    }

    if(ms.empty())
    {
        return generateFreeOperatorCall(c, node, first, params, op, fs.front());
    }
    else
    {
        return generateMethodOperatorCall(c, node, first, params, op, ms.front());
    }
}


