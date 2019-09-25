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

#include "decorator/ExprDecorator.h"

#include "transform/ExprTransform.h"

#include "visitors/TypeVisitor.h"
#include "visitors/QueryVisitors.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

FuncTransform::FuncTransform(Context &c) : c(c), index(0)
{
}

void FuncTransform::visit(BlockNode &node)
{
    for(std::size_t i = 0; i < node.nodes.size(); ++i)
    {
        index = i;
        node.nodes[i]->accept(*this);
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
        auto en = new ExprNode(node.location());
        node.block()->insert(index + 1, en);

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

        auto t = Type::makeFunction(c.types.nullType(), { c.types.insert(Type::makePrimary(false, true, type->sym)) });

        for(auto &p: params)
        {
            t.args.push_back(TypeVisitor::assertType(c, p.get()));
        }

        NodePtr target(new IdNode(node.location(), node.name, "new"));
        target = ExprDecorator::decorate(c, target, c.types.insert(t));

        auto cn = new CallNode(node.location(), target);
        en->expr = cn;

        for(auto &p: params)
        {
            cn->params.push_back(p);
        }
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
