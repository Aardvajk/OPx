#include "CommonDecorator.h"

#include "application/Context.h"

#include "nodes/FuncNode.h"

#include "types/Type.h"
#include "types/TypeCompare.h"
#include "types/TypeBuilder.h"
#include "types/TypeLookup.h"

#include "visitors/TypeVisitor.h"
#include "visitors/SymFinder.h"
#include "visitors/NameVisitors.h"

#include <unordered_set>

namespace
{

Sym *searchCallableLocal(Context &c, FuncNode &node, const Type *type)
{
    std::vector<Sym*> sv;
    SymFinder::find(SymFinder::Type::Local, c.tree.current(), node.name.get(), sv);

    for(auto s: sv)
    {
        if(s->type() != Sym::Type::Func)
        {
            throw Error(node.location(), "function expected - ", s->fullname());
        }

        if(TypeCompare::compatibleArgs(type, s->property<const Type*>("type")))
        {
            return s;
        }
    }

    return nullptr;
}

std::vector<Sym*> searchCallable(Location location, const std::vector<Sym*> &sv, const Type *expectedType)
{
    std::vector<Sym*> rs;

    for(auto s: sv)
    {
        if(s->type() == Sym::Type::Class)
        {
            rs.push_back(s);
        }
        else
        {
            if(!s->property<const Type*>("type")->function())
            {
                throw Error(location, "callable expected - ", s->fullname());
            }

            if(TypeCompare::compatibleArgs(expectedType, s->property<const Type*>("type")))
            {
                rs.push_back(s);
            }
        }
    }

    return rs;
}

void checkResult(Node &node, const std::vector<Sym*> &rs, const Type *expectedType)
{
    if(rs.empty())
    {
        throw Error(node.location(), "no function matched - ", NameVisitors::prettyName(&node), expectedType->text());
    }

    if(rs.size() > 1)
    {
        throw Error(node.location(), "ambigous reference - ", NameVisitors::prettyName(&node), expectedType->text());
    }
}

}

Sym *CommonDecorator::searchCallableByType(Context &c, Node &node, const Type *expectedType)
{
    std::unordered_set<Sym*> search;
    for(auto &a: expectedType->args)
    {
        if(a->sym && a->sym->parent() != c.tree.root())
        {
            search.insert(a->sym->parent());
        }
    }

    std::vector<Sym*> sv;
    SymFinder::find(SymFinder::Type::Global, c.tree.current(), &node, sv);

    for(auto s: search)
    {
        SymFinder::find(SymFinder::Type::Local, s, &node, sv);
    }

    auto rs = searchCallable(node.location(), sv, expectedType);
    checkResult(node, rs, expectedType);

    auto sym = rs.front();
    if(sym->type() == Sym::Type::Class)
    {
        std::vector<Sym*> v;
        for(auto s: sym->children())
        {
            if(s->type() == Sym::Type::Func && s->name() == "new")
            {
                if(TypeCompare::compatibleArgs(expectedType, s->property<const Type*>("type")))
                {
                    v.push_back(s);
                }
            }
        }

        checkResult(node, v, expectedType);
        node.setProperty("newmethod", v.front());
    }

    return sym;
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

    Sym *sym = searchCallableLocal(c, node, type);
    if(sym)
    {
        auto st = sym->property<const Type*>("type");

        if(!TypeCompare::exactArgs(type, st))
        {
            throw Error(node.location(), "mismatched parameter references");
        }

        if(!TypeCompare::exact(type->returnType, st->returnType))
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

    if(NameVisitors::isNameSpecial(node.name.get()) != Token::Type::Invalid)
    {
        if(!sym->parent() || sym->parent()->type() != Sym::Type::Class)
        {
            throw Error(node.location(), "cannot declare outside class - ", NameVisitors::prettyName(node.name.get()));
        }
    }

    return sym;
}
