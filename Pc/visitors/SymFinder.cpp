#include "SymFinder.h"

#include "nodes/IdNode.h"
#include "nodes/DotNode.h"

#include "symbols/Sym.h"

namespace
{

std::vector<SymFinder::Result> &findIn(SymFinder::Policy policy, Sym *scope, const std::string &name, std::vector<SymFinder::Result> &result)
{
    for(auto s: scope->children())
    {
        if(s->type() == Sym::Type::UsingScope && policy != SymFinder::Policy::Declaration)
        {
            findIn(policy, s->property("proxy").to<Sym*>(), name, result);
        }

        if(s->name() == name)
        {
            result.push_back({ s->resolved(), true });
        }
    }

    return result;
}

void findFirst(SymFinder::Policy policy, Sym *scope, const std::string &name, std::vector<SymFinder::Result> &result)
{
    while(scope)
    {
        if(!findIn(policy, scope, name, result).empty())
        {
            return;
        }

        scope = scope->parent();
    }
}

}

SymFinder::SymFinder(Policy policy, Sym *start) : policy(policy), start(start)
{
}

void SymFinder::visit(IdNode &node)
{
    if(scopes.empty())
    {
        findFirst(policy, start, node.name, v);
    }
    else
    {
        for(auto s: scopes)
        {
            findIn(policy, s.sym, node.name, v);
        }
    }
}

void SymFinder::visit(DotNode &node)
{
    if(scopes.empty())
    {
        findFirst(policy, start, node.name, scopes);
    }
    else
    {
        std::vector<Result> v;
        for(auto s: scopes)
        {
            findIn(policy, s.sym, node.name, v);
            scopes = v;
        }
    }

    node.child->accept(*this);
}
