#include "CommonConvert.h"

#include "application/Context.h"

#include "nodes/ProxyCallNode.h"
#include "nodes/ConstructNode.h"

#include "nodes/TypeCastNode.h"

#include "types/Type.h"
#include "types/TypeCompare.h"
#include "types/TypeConvert.h"

#include "decorator/ExprDecorator.h"

#include "visitors/TypeVisitor.h"

NodePtr CommonConvert::convert(Context &c, NodePtr node, Type *type, TypeConvert::Permission permission)
{
    auto t = TypeVisitor::assertType(c, node.get());

    if(!TypeCompare(c).compatible(type, t))
    {
        auto sv = TypeConvert::find(c, t, type, permission);
        if(sv.empty())
        {
            throw Error(node->location(), "no conversion found - ", node->description());
        }

        if(sv.size() > 1)
        {
            throw Error(node->location(), "ambiguous conversion - ", node->description());
        }

        auto sym = sv.front();

        if(!sym->accessibleFrom(c.tree.current()))
        {
            throw Error(node->location(), "not accessible - ", sym->funcname());
        }

        if(sym->name() == "new")
        {
            type = c.types.insert(type->removeReference());

            auto cn = new ConstructNode(node->location(), type);
            NodePtr n(cn);

            cn->setProperty("type", type);
            cn->params.push_back(node);

            node = n;
        }
        else if(sym->property<Type*>("type")->method)
        {
            auto cn = new ProxyCallNode(node->location(), sym, node);
            node = cn;
        }
        else
        {
            auto cn = new ProxyCallNode(node->location(), sym);
            NodePtr n(cn);

            cn->params.push_back(node);
            node = n;
        }

        node = ExprDecorator::decorate(c, node);
    }

    return node;
}

NodePtr CommonConvert::convertImplicitBoolean(Context &c, NodePtr node)
{
    auto t = TypeVisitor::assertType(c, node.get());

    if(t->primitive() && !TypeCompare(c).compatible(t, c.types.boolType()))
    {
        auto n = new ConstructNode(node->location(), c.types.boolType());
        NodePtr nn(n);

        n->setProperty("type", c.types.boolType());
        n->params.push_back(node);

        return nn;
    }

    return convert(c, node, c.types.boolType(), TypeConvert::Permission::Implicit);
}
