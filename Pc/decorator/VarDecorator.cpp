#include "VarDecorator.h"

#include "application/Context.h"

#include "nodes/VarNode.h"

#include "decorator/TypeDecorator.h"
#include "decorator/ExprDecorator.h"

#include "visitors/TypeVisitor.h"
#include "visitors/NameVisitors.h"
#include "visitors/SymFinder.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

namespace
{

Sym *search(Context &c, Node *node)
{
    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Local, c.tree.current(), node, sv);

    for(auto s: sv)
    {
        if(s->type() != Sym::Type::Var)
        {
            throw Error(node->location(), "var expected - ", s->fullname());
        }

        return s;
    }

    return nullptr;
}

}

VarDecorator::VarDecorator(Context &c) : c(c)
{
}

void VarDecorator::visit(VarNode &node)
{
    Type *type = nullptr;

    if(node.type)
    {
        Visitor::visit<TypeDecorator>(node.type.get(), c);
        type = TypeVisitor::assertType(c, node.type.get());
    }

    if(node.value)
    {
        node.value = ExprDecorator::decorate(c, node.value);
        if(!type)
        {
            type = TypeVisitor::assertType(c, node.value.get());

            if(type->ref)
            {
                auto t = *type;
                t.ref = false;

                type = c.types.insert(t);
            }
        }
    }

    auto sym = search(c, node.name.get());
    if(sym)
    {
        if(!sym->findProperty("external").value<bool>() && !node.findProperty("external").value<bool>())
        {
            throw Error(node.location(), "already defined - ", sym->fullname());
        }

        if(type && !TypeCompare(c).exact(type, sym->property<Type*>("type")))
        {
            throw Error(node.location(), "mismatched type - ", sym->fullname());
        }
    }
    else
    {
        if(!type)
        {
            throw Error(node.location(), "no type specified - ", node.name->description());
        }

        auto name = NameVisitors::assertSimpleUniqueName(c, node.name.get());
        if(name.empty())
        {
            auto info = c.tree.current()->container()->property<FuncInfo*>("info");
            name = pcx::str("#unnamed", info->labels++);
        }

        sym = c.tree.current()->add(new Sym(Sym::Type::Var, node.location(), node.property<Access>("access"), name));

        sym->setProperty("type", type);
        sym->setProperty("member", c.tree.current()->type() == Sym::Type::Class && !node.findProperty("free").value<bool>());
        sym->setProperty("external", node.findProperty("external").value<bool>());
        sym->setProperty("free", node.findProperty("free").value<bool>());
    }

    node.setProperty("sym", sym);
}
