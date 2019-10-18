#include "ExprTransform.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/FuncNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/ProxyCallNode.h"
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
#include "nodes/TernaryNode.h"
#include "nodes/TypeCastNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include "decorator/ExprDecorator.h"

#include "transform/FuncTransform.h"

#include "visitors/TypeVisitor.h"
#include "visitors/QueryVisitors.h"

#include <pcx/indexed_range.h>
#include <pcx/scoped_counter.h>

namespace
{

template<typename T> void transformLiteral(Context &c, T &node, Type *expectedType)
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

template<typename T> void transformDefaultParams(Context &c, T &node, Type *type, Sym *sym, std::size_t offset = 0)
{
    if(auto fn = sym->findProperty("funcnode").value<FuncNode*>())
    {
        while(node.params.size() + offset < type->args.size())
        {
            auto value = Visitor::query<QueryVisitors::GetVarValue, NodePtr>(fn->args[node.params.size() + offset].get());

            value = ExprTransform::transform(c, value);
            node.params.push_back(value);
        }
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

void ExprTransform::visit(SizeLiteralNode &node)
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

    if(auto s = node.target->findProperty("sym"))
    {
        transformDefaultParams(c, node, type, s.to<Sym*>());
    }

    for(auto p: pcx::indexed_range(node.params))
    {
        p.value = ExprTransform::transform(c, p.value, type->args[p.index]);
    }
}

void ExprTransform::visit(ProxyCallNode &node)
{
    auto type = node.sym->property<Type*>("type");

    if(type->method)
    {
        node.params.insert(node.params.begin(), node.thisNode);
    }

    transformDefaultParams(c, node, type, node.sym);

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

    if(node.type->primitive())
    {
        transformLiteral(c, node, expectedType);
    }
    else
    {
        auto t = Type::makeFunction(c.types.nullType(), { c.types.insert(Type::makePrimary(false, true, node.type->sym)) });
        for(auto &p: node.params)
        {
            t.args.push_back(TypeVisitor::assertType(c, p.get()));
        }

        NodePtr name(IdNode::create(node.location(), node.type->sym->names()));
        node.target = new IdNode(node.location(), name, "new");

        node.target = ExprDecorator::decorate(c, node.target, c.types.insert(t));

        if(c.tree.current()->container()->type() == Sym::Type::Func)
        {
            auto info = c.tree.current()->container()->property<FuncInfo*>("info");
            auto temp = pcx::str("temp", info->labels++);

            info->temps.push_back(std::make_pair(temp, node.type));
            node.setProperty("temp", temp);
        }

        if(auto sp = node.target->findProperty("sym"))
        {
            transformDefaultParams(c, node, TypeVisitor::assertType(c, node.target.get()), sp.to<Sym*>(), 1);
        }
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
    auto g = pcx::scoped_counter(c.potentiallySkipped);

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
    node.left = ExprTransform::transform(c, node.left);
    node.right = ExprTransform::transform(c, node.right);
}

void ExprTransform::visit(InlineVarNode &node)
{
    Visitor::visit<FuncTransform>(node.body.get(), c);
}

void ExprTransform::visit(TernaryNode &node)
{
    node.expr = ExprTransform::transform(c, node.expr);

    auto g = pcx::scoped_counter(c.potentiallySkipped);

    node.left = ExprTransform::transform(c, node.left);
    node.right = ExprTransform::transform(c, node.right);
}

void ExprTransform::visit(TypeCastNode &node)
{
    node.expr = ExprTransform::transform(c, node.expr);
}

NodePtr ExprTransform::transform(Context &c, NodePtr &node, Type *expectedType)
{
    ExprTransform et(c, expectedType);
    node->accept(et);

    return et.result() ? et.result() : node;
}
