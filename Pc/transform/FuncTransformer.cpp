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

#include "visitors/NameVisitors.h"
#include "visitors/TypeVisitor.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include "transform/ExprTransformer.h"

#include "decorator/ExprDecorator.h"

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
    if(node.value)
    {
        auto type = node.property<const Sym*>("sym")->property<const Type*>("type");

        if(type->primitive())
        {
            if(!TypeCompare::exact(type, TypeVisitor::type(c, node.value.get())))
            {
                throw Error(node.value->location(), type->text(), " expected - ", NameVisitors::prettyName(node.value.get()));
            }

            auto en = new ExprNode(node.location(), { });
            node.block()->insert(index + 1, en);

            auto an = new AssignNode(node.location(), node.name);

            en->expr = an;
            an->expr = node.value;

            ExprDecorator::decorate(c, nullptr, *an);
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
