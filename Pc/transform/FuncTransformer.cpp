#include "FuncTransformer.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/VarNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ExprNode.h"
#include "nodes/ReturnNode.h"
#include "nodes/CallNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/AssignNode.h"
#include "nodes/WhileNode.h"
#include "nodes/IfNode.h"

#include "visitors/NameVisitors.h"
#include "visitors/TypeVisitor.h"

#include "types/Type.h"
#include "types/TypeCompare.h"
#include "types/TypeLookup.h"

#include "transform/ExprTransformer.h"

#include "decorator/ExprDecorator.h"
#include "decorator/FuncDecorator.h"

namespace
{

void generatePrimitiveAssign(Context &c, Type *type, VarNode &node, NodePtr &value, std::size_t index)
{
    if(!TypeCompare::exact(type, TypeVisitor::type(c, value.get())))
    {
        throw Error(value->location(), type->text(), " expected - ", NameVisitors::prettyName(value.get()));
    }

    auto en = new ExprNode(value->location(), { });
    node.block()->insert(index + 1, en);

    auto an = new AssignNode(node.location(), node.name);

    en->expr = an;
    an->expr = value;

    ExprDecorator::decorate(c, nullptr, *an);
}

void generateNonPrimitiveAssign(Context &c, Type *type, VarNode &node, std::size_t index)
{
    auto en = new ExprNode(node.location(), { });
    node.block()->insert(index + 1, en);

    auto cn = new CallNode(node.location(), new IdNode(node.location(), node.name, "new"));
    en->expr = cn;

    std::vector<Type*> tv;
    for(auto &p: node.params)
    {
        cn->params.push_back(p);
        tv.push_back(TypeVisitor::type(c, p.get()));
    }

    cn->target->setProperty("sym", TypeLookup::assertNewMethod(c, node.location(), type, tv));
}

}

FuncTransformer::FuncTransformer(Context &c) : c(c), index(0)
{
}

void FuncTransformer::visit(BlockNode &node)
{
    for(std::size_t i = 0; i < node.nodes.size(); ++i)
    {
        index = i;
        node.nodes[i]->accept(*this);
    }
}

void FuncTransformer::visit(VarNode &node)
{
    auto type = node.property<Sym*>("sym")->property<Type*>("type");

    if(!node.params.empty())
    {
        if(type->primitive())
        {
            if(node.params.size() > 1)
            {
                throw Error(node.location(), "invalid number of parameters");
            }

            generatePrimitiveAssign(c, type, node, node.params.front(), index);
        }
        else
        {
            generateNonPrimitiveAssign(c, type, node, index);
        }
    }

    if(node.value)
    {
        if(type->primitive())
        {
            generatePrimitiveAssign(c, type, node, node.value, index);
        }
        else
        {
            auto en = new ExprNode(node.location(), { });
            node.block()->insert(index + 1, en);

            auto cn = new CallNode(node.location(), new IdNode(node.location(), { }, "operator="));
            en->expr = cn;

            cn->params.push_back(new AddrOfNode(node.location(), node.name));
            cn->params.push_back(node.value);

            ExprDecorator::decorate(c, nullptr, *cn);
        }
    }
    else if(!type->primitive())
    {
        if(node.params.empty())
        {
            generateNonPrimitiveAssign(c, type, node, index);
        }
    }
}

void FuncTransformer::visit(ScopeNode &node)
{
    auto g = c.tree.open(node.property<Sym*>("sym"));
    node.body->accept(*this);
}

void FuncTransformer::visit(ExprNode &node)
{
    node.expr = ExprTransformer::transform(c, node.expr);
}

void FuncTransformer::visit(ReturnNode &node)
{
    node.expr = ExprTransformer::transform(c, node.expr);
}

void FuncTransformer::visit(WhileNode &node)
{
    node.expr = ExprTransformer::transform(c, node.expr);
    node.body->accept(*this);
}

void FuncTransformer::visit(IfNode &node)
{
    node.expr = ExprTransformer::transform(c, node.expr);
    node.body->accept(*this);

    if(node.elseBody)
    {
        node.elseBody->accept(*this);
    }
}
