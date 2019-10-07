#include "CommonDecorator.h"

#include "application/Context.h"

#include "syms/Sym.h"

#include "types/Type.h"
#include "types/TypeCompare.h"
#include "types/TypeConvert.h"

#include "visitors/SymFinder.h"

#include <map>
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
        std::map<std::size_t, std::vector<Sym*> > m;

        for(auto s: sv)
        {
            if(s->type() == Sym::Type::Func)
            {
                auto type = s->property<Type*>("type");

                if(type->args.size() == expectedType->args.size())
                {
                    std::size_t matches = 0;
                    std::size_t conversions = 0;

                    for(std::size_t i = 0; i < type->args.size(); ++i)
                    {
                        if(TypeCompare(c).compatible(expectedType->args[i], type->args[i]))
                        {
                            ++matches;
                        }
                        else if(!TypeConvert::find(c, expectedType->args[i], type->args[i], TypeConvert::Permission::Implicit).empty())
                        {
                            ++matches;
                            ++conversions;
                        }
                    }

                    if(matches == type->args.size())
                    {
                        m[conversions].push_back(s);
                    }
                }
            }
            else
            {
                m[0].push_back(s);
            }
        }

        sv.clear();
        if(!m.empty())
        {
            sv = m.begin()->second;
        }
    }

    if(sv.size() > 1 && expectedType)
    {
        sv = pruneResult(sv, expectedType);
    }

    return sv;
}

