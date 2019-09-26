#include "FuncTransform.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/CallNode.h"
#include "nodes/ConstructNode.h"
#include "nodes/VarNode.h"
#include "nodes/ExprNode.h"
#include "nodes/ReturnNode.h"
#include "nodes/InitNode.h"
#include "nodes/AssignNode.h"

#include "decorator/ExprDecorator.h"

#include "transform/ExprTransform.h"

#include "visitors/TypeVisitor.h"
#include "visitors/QueryVisitors.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

namespace
{

NodePtr createComplexConstruct(Context &c, Type *type, Node &node, NodePtr &name, NodeList &params)
{
    auto en = new ExprNode(node.location());
    NodePtr rn(en);

    auto t = Type::makeFunction(c.types.nullType(), { c.types.insert(Type::makePrimary(false, true, type->sym)) });

    for(auto &p: params)
    {
        t.args.push_back(TypeVisitor::assertType(c, p.get()));
    }

    NodePtr target(new IdNode(node.location(), name, "new"));
    target = ExprDecorator::decorate(c, target, c.types.insert(t));

    auto cn = new CallNode(node.location(), target);
    en->expr = cn;

    for(auto &p: params)
    {
        cn->params.push_back(p);
    }

    return rn;
}

}

FuncTransform::FuncTransform(Context &c) : c(c), index(0)
{
}

void FuncTransform::visit(BlockNode &node)
{
    for(std::size_t i = 0; i < node.nodes.size(); ++i)
    {
        index = i;
        node.nodes[i] = FuncTransform::transform(c, node.nodes[i]);
    }
}

void FuncTransform::visit(ScopeNode &node)
{
    auto sg = c.tree.open(node.property<Sym*>("sym"));
    node.body->accept(*this);
}

void FuncTransform::visit(VarNode &node)
{
    if(node.value)
    {
        node.value = ExprTransform::transform(c, node.value);
    }

    auto type = TypeVisitor::assertType(c, &node);
    if(!type->primitiveOrRef())
    {
        NodeList params;
        if(node.value)
        {
            auto cn = Visitor::query<QueryVisitors::GetConstructNode, ConstructNode*>(node.value.get());
            if(cn && TypeCompare(c).compatible(type, cn->type) && c.option("O", "elide_constructor_copies"))
            {
                params = cn->params;
            }
            else
            {
                params.push_back(node.value);
            }
        }

        node.block()->insert(index + 1, createComplexConstruct(c, type, node, node.name, params));
    }
}

void FuncTransform::visit(ExprNode &node)
{
    node.expr = ExprTransform::transform(c, node.expr);
}

void FuncTransform::visit(ReturnNode &node)
{
    if(node.expr)
    {
        node.expr = ExprTransform::transform(c, node.expr);
    }
}

void FuncTransform::visit(InitNode &node)
{
    node.target = ExprTransform::transform(c, node.target);

    for(auto &p: node.params)
    {
        p = ExprTransform::transform(c, p);
    }

    auto type = TypeVisitor::assertType(c, &node);
    if(type->primitiveOrRef())
    {
        if(node.params.size() > 1)
        {
            throw Error(node.location(), "too many parameters - ", node.description());
        }

        if(type->ref && node.params.empty())
        {
            throw Error(node.location(), "reference must be initialised - ", node.description());
        }

        auto en = new ExprNode(node.location());
        rn = en;

        auto an = new AssignNode(node.location(), node.target);
        en->expr = an;

        an->expr = new ConstructNode(node.location(), type, node.params);
    }
    else
    {
        rn = createComplexConstruct(c, type, node, node.target, node.params);
    }

    rn = FuncTransform::transform(c, rn);
}

NodePtr FuncTransform::transform(Context &c, NodePtr &node)
{
    FuncTransform ft(c);
    node->accept(ft);

    return ft.result() ? ft.result() : node;
}
