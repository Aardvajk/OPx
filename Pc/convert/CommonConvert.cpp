#include "CommonConvert.h"

#include "application/Context.h"

#include "nodes/ProxyCallNode.h"
#include "nodes/ConstructNode.h"

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
        else
        {
            auto cn = new ProxyCallNode(node->location(), sym, node);
            node = cn;
        }

        node = ExprDecorator::decorate(c, node);
    }

    return node;
}
