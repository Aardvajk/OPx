#include "ClassDecorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"
#include "nodes/FuncNode.h"

#include "types/Type.h"
#include "types/TypeBuilder.h"

#include "visitors/NameVisitors.h"
#include "visitors/TypeVisitor.h"

#include "decorator/Decorator.h"
#include "decorator/CommonDecorator.h"

ClassDecorator::ClassDecorator(Context &c) : c(c)
{
}

void ClassDecorator::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void ClassDecorator::visit(ClassNode &node)
{
    Decorator d(c);
    node.accept(d);
}

void ClassDecorator::visit(VarNode &node)
{
    if(!node.type)
    {
        throw Error(node.location(), "type missing - ", NameVisitors::prettyName(node.name.get()));
    }

    auto type = TypeBuilder::type(c, node.type.get());

    auto name = c.assertSimpleNameUnique(node.name.get());

    auto sym = c.tree.current()->add(new Sym(Sym::Type::Var, node.name->location(), name));
    sym->setProperty("type", type);
    sym->setProperty("member", true);

    node.setProperty("sym", sym);
}

void ClassDecorator::visit(FuncNode &node)
{
    auto sym = CommonDecorator::decorateFuncSignature(c, node);
    sym->setProperty("method", true);

    node.setProperty("sym", sym);

    if(node.body)
    {
        c.deferredMethods.push_back(&node);
    }
}
