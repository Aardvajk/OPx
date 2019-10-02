#include "ExprTransform.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/ConstructNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/DerefNode.h"
#include "nodes/AssignNode.h"
#include "nodes/UnaryNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/LogicalNode.h"
#include "nodes/ThisNode.h"
#include "nodes/IncDecNodes.h"
#include "nodes/CommaNode.h"
#include "nodes/InlineVarNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include "decorator/ExprDecorator.h"

#include "transform/FuncTransform.h"

#include "visitors/TypeVisitor.h"
#include "visitors/QueryVisitors.h"

#include <pcx/indexed_range.h>

namespace
{

void transformLiteral(Context &c, Node &node, Type *expectedType)
{
    if(expectedType && expectedType->ref)
    {
        auto info = c.tree.current()->container()->property<FuncInfo*>("info");

        auto temp = pcx::str("temp_literal", info->labels++);

        node.setProperty("temp_literal", temp);
        info->temps.push_back(std::make_pair(temp, TypeVisitor::assertType(c, &node)));
    }
}

template<typename T> void transformIncDec(Context &c, T &node)
{
    node.expr = ExprTransform::transform(c, node.expr);

    if(!Visitor::query<QueryVisitors::IsMutable, bool>(node.expr.get(), c))
    {
        throw Error(node.location(), "cannot modify a constant - ", node.description());
    }
}

}

ExprTransform::ExprTransform(Context &c, Type *expectedType) : c(c), expectedType(expectedType)
{
}

void ExprTransform::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent = ExprTransform::transform(c, node.parent);
    }
    else
    {
        if(auto s = node.findProperty("sym"))
        {
            auto sym = s.to<Sym*>();

            if(sym->findProperty("member").value<bool>() || sym->findProperty("method").value<bool>())
            {
                if(!c.tree.current()->container()->findProperty("method").value<bool>())
                {
                    throw Error(node.location(), "cannot access in free method - ", node.description());
                }

                auto n = new ThisNode(node.location());
                node.parent = n;

                auto t = Type::makePrimary(sym->parent());

                t.ref = true;
                t.constant = c.tree.current()->container()->property<Type*>("type")->constMethod;

                n->setProperty("type", c.types.insert(t));
            }
        }
    }
}

void ExprTransform::visit(CharLiteralNode &node)
{
    transformLiteral(c, node, expectedType);
}

void ExprTransform::visit(IntLiteralNode &node)
{
    transformLiteral(c, node, expectedType);
}

void ExprTransform::visit(BoolLiteralNode &node)
{
    transformLiteral(c, node, expectedType);
}

void ExprTransform::visit(CallNode &node)
{
    node.target = ExprTransform::transform(c, node.target);

    auto type = TypeVisitor::assertType(c, node.target.get());

    if(type->method)
    {
        node.params.insert(node.params.begin(), Visitor::query<QueryVisitors::GetParent, NodePtr>(node.target.get()));
    }

    for(auto p: pcx::indexed_range(node.params))
    {
        p.value = ExprTransform::transform(c, p.value, type->args[p.index]);
    }
}

void ExprTransform::visit(ConstructNode &node)
{
    for(auto &p: node.params)
    {
        p = ExprTransform::transform(c, p);
    }

    if(!node.type->primitive())
    {
        auto t = Type::makeFunction(c.types.nullType(), { c.types.insert(Type::makePrimary(false, true, node.type->sym)) });
        for(auto &p: node.params)
        {
            t.args.push_back(TypeVisitor::assertType(c, p.get()));
        }

        NodePtr name(IdNode::create(node.location(), node.type->sym->names()));
        node.target = new IdNode(node.location(), name, "new");

        node.target = ExprDecorator::decorate(c, node.target, c.types.insert(t));

        auto info = c.tree.current()->container()->property<FuncInfo*>("info");
        auto temp = pcx::str("temp", info->labels++);

        info->temps.push_back(std::make_pair(temp, node.type));
        node.setProperty("temp", temp);
    }
}

void ExprTransform::visit(AddrOfNode &node)
{
    node.expr = ExprTransform::transform(c, node.expr);
}

void ExprTransform::visit(DerefNode &node)
{
    node.expr = ExprTransform::transform(c, node.expr);
}

void ExprTransform::visit(AssignNode &node)
{
    node.target = ExprTransform::transform(c, node.target);
    node.expr = ExprTransform::transform(c, node.expr);

    if(!Visitor::query<QueryVisitors::IsMutable, bool>(node.target.get(), c))
    {
        throw Error(node.location(), "cannot modify a constant - ", node.description());
    }
}

void ExprTransform::visit(UnaryNode &node)
{
    node.expr = ExprTransform::transform(c, node.expr);
}

void ExprTransform::visit(BinaryNode &node)
{
    node.left = ExprTransform::transform(c, node.left);
    node.right = ExprTransform::transform(c, node.right);
}

void ExprTransform::visit(LogicalNode &node)
{
    node.left = ExprTransform::transform(c, node.left);
    node.right = ExprTransform::transform(c, node.right);
}

void ExprTransform::visit(PreIncDecNode &node)
{
    transformIncDec(c, node);
}

void ExprTransform::visit(PostIncDecNode &node)
{
    transformIncDec(c, node);
}

void ExprTransform::visit(CommaNode &node)
{
    node.first = ExprTransform::transform(c, node.first);
    node.second = ExprTransform::transform(c, node.second);
}

void ExprTransform::visit(InlineVarNode &node)
{
    Visitor::visit<FuncTransform>(node.body.get(), c);
}

NodePtr ExprTransform::transform(Context &c, NodePtr &node, Type *expectedType)
{
    ExprTransform et(c, expectedType);
    node->accept(et);

    return et.result() ? et.result() : node;
}
