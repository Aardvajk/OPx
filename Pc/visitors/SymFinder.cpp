#include "SymFinder.h"

#include "nodes/IdNode.h"
#include "nodes/DotNode.h"

#include "symbols/Sym.h"

namespace
{

std::vector<Sym*> &findIn(Sym *scope, const std::string &name, std::vector<Sym*> &result)
{
    for(auto s: scope->children())
    {
        if(s->type() == Sym::Type::UsingClass)
        {
            findIn(s->property("proxy").to<Sym*>(), name, result);
        }

        if(s->name() == name)
        {
            result.push_back(s->resolved());
        }
    }

    return result;
}

void findFirst(Sym *scope, Sym *limit, const std::string &name, std::vector<Sym*> &result)
{
    while(scope != limit)
    {
        if(!findIn(scope, name, result).empty())
        {
            return;
        }

        scope = scope->parent();
    }
}

}

SymFinder::SymFinder(Sym *start, Sym *limit) : start(start), limit(limit)
{
}

void SymFinder::visit(IdNode &node)
{
    if(scopes.empty())
    {
        findFirst(start, limit, node.name, v);
    }
    else
    {
        for(auto s: scopes)
        {
            findIn(s, node.name, v);
        }
    }
}

void SymFinder::visit(DotNode &node)
{
    if(scopes.empty())
    {
        findFirst(start, limit, node.name, scopes);
    }
    else
    {
        std::vector<Sym*> v;
        for(auto s: scopes)
        {
            findIn(s, node.name, v);
            scopes = v;
        }
    }

    node.child->accept(*this);
}
