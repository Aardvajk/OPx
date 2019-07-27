#include "CommonDecorator.h"

#include "application/Context.h"

#include "nodes/FuncNode.h"

#include "types/Type.h"
#include "types/TypeCompare.h"
#include "types/TypeBuilder.h"

#include "visitors/TypeVisitor.h"
#include "visitors/SymFinder.h"
#include "visitors/NameVisitors.h"

namespace
{

Sym *searchFunction(Context &c, FuncNode &node, const Type *type)
{
    std::vector<Sym*> sv;
    SymFinder::find(SymFinder::Type::Local, c.tree.current(), node.name.get(), sv);

    for(auto s: sv)
    {
        if(s->type() != Sym::Type::Func)
        {
            throw Error(node.location(), "function expected - ", s->fullname());
        }

        if(TypeCompare::args(type, s->property<const Type*>("type")))
        {
            return s;
        }
    }

    return nullptr;
}

}

Sym *CommonDecorator::decorateFuncSignature(Context &c, FuncNode &node)
{
    auto t = Type::makeFunction(0, node.type ? TypeBuilder::type(c, node.type.get()) : c.types.nullType());
    for(auto &a: node.args)
    {
        t.args.push_back(TypeVisitor::type(c, a.get()));

        if(t.args.back()->sub)
        {
            throw Error(a->location(), "arrays cannot be arguments - ", NameVisitors::prettyName(a.get()));
        }
    }

    auto type = c.types.insert(t);

    Sym *sym = searchFunction(c, node, type);
    if(sym)
    {
        if(!TypeCompare::exact(type->returnType, sym->property<const Type*>("type")->returnType))
        {
            throw Error(node.location(), "mismatched returns");
        }
    }
    else
    {
        auto name = c.assertSimpleName(node.name.get());

        sym = c.tree.current()->add(new Sym(Sym::Type::Func, node.location(), name));
        sym->setProperty("type", type);
    }

    return sym;
}
