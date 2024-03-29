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
#include "nodes/WhileNode.h"
#include "nodes/IfNode.h"
#include "nodes/ForNode.h"

#include "decorator/ExprDecorator.h"

#include "transform/ExprTransform.h"

#include "visitors/TypeVisitor.h"
#include "visitors/QueryVisitors.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include "decorator/CommonDecorator.h"
#include "syms/Sym.h"

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

FuncTransform::FuncTransform(Context &c, std::size_t index) : c(c), index(index)
{
}

void FuncTransform::visit(BlockNode &node)
{
    for(std::size_t i = 0; i < node.nodes.size(); ++i)
    {
        node.nodes[i] = FuncTransform::transform(c, node.nodes[i], i);
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

        NodePtr id = node.findProperty("globalinit").value<bool>() ? IdNode::create(node.location(), node.property<Sym*>("sym")->names()) : node.name;

        node.block()->insert(index + 1, createComplexConstruct(c, type, node, id, params));
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

void FuncTransform::visit(WhileNode &node)
{
    node.expr = ExprTransform::transform(c, node.expr);
    node.body->accept(*this);
}

void FuncTransform::visit(IfNode &node)
{
    node.expr = ExprTransform::transform(c, node.expr);
    node.body->accept(*this);

    if(node.elseBody)
    {
        node.elseBody->accept(*this);
    }
}

void FuncTransform::visit(ForNode &node)
{
    if(node.init)
    {
        node.init = ExprTransform::transform(c, node.init);
    }

    if(node.cond)
    {
        node.cond = ExprTransform::transform(c, node.cond);
    }

    if(node.post)
    {
        node.post = ExprTransform::transform(c, node.post);
    }

    node.body->accept(*this);
}

NodePtr FuncTransform::transform(Context &c, NodePtr node, std::size_t index)
{
    FuncTransform ft(c, index);
    node->accept(ft);

    return ft.result() ? ft.result() : node;
}
