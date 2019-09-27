#include "CommonDecorator.h"

#include "application/Context.h"

#include "syms/Sym.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include "visitors/SymFinder.h"

#include <unordered_set>

namespace
{

std::vector<Sym*> pruneResult(const std::vector<Sym*> &sv, const Type *expectedType)
{
    std::vector<Sym*> r;

    for(auto s: sv)
    {
        auto type = s->property<Type*>("type");

        if(type->constMethod == expectedType->constMethod)
        {
            r.push_back(s);
        }
    }

    return r;
}

}

std::vector<Sym*> CommonDecorator::searchCallable(Context &c, Node *node, Type *expectedType)
{
    std::vector<Sym*> sv;
    std::unordered_set<Sym*> search;

    for(auto &a: expectedType->args)
    {
        if(a->sym && a->sym->parent() != c.tree.root())
        {
            search.insert(a->sym->parent());
        }
    }

    SymFinder::find(c, SymFinder::Type::Global, c.tree.current(), node, sv);

    for(auto s: search)
    {
        SymFinder::find(c, SymFinder::Type::Local, s, node, sv);
    }

    if(expectedType && expectedType->function())
    {
        std::vector<Sym*> r;

        for(auto s: sv)
        {
            if(s->type() == Sym::Type::Func)
            {
                auto type = s->property<Type*>("type");

                if(TypeCompare(c).compatibleArgs(type, expectedType) && (!expectedType->constMethod || type->constMethod))
                {
                    r.push_back(s);
                }
            }
            else
            {
                r.push_back(s);
            }
        }

        sv = r;
    }

    if(sv.size() > 1 && expectedType)
    {
        sv = pruneResult(sv, expectedType);
    }

    return sv;
}

