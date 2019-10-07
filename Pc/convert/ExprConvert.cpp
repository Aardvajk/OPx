#include "ExprConvert.h"

#include "nodes/CallNode.h"
#include "nodes/ProxyCallNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include "convert/CommonConvert.h"

#include "visitors/TypeVisitor.h"

namespace
{

void convertParams(Context &c, NodeList &params, Type *type)
{
    for(std::size_t i = 0; i < params.size(); ++i)
    {
        params[i] = CommonConvert::convert(c, params[i], type->args[i]);
    }
}

}

ExprConvert::ExprConvert(Context &c, NodePtr &cn, Type *expectedType) : c(c), cn(cn), expectedType(expectedType)
{
}

void ExprConvert::visit(CallNode &node)
{
    convertParams(c, node.params, TypeVisitor::assertType(c, node.target.get()));
}

void ExprConvert::visit(ProxyCallNode &node)
{
    convertParams(c, node.params, node.sym->property<Type*>("type"));
}

NodePtr ExprConvert::convert(Context &c, NodePtr &node, Type *expectedType)
{
    ExprConvert ec(c, node, expectedType);
    node->accept(ec);

    return ec.result() ? ec.result() : node;
}
