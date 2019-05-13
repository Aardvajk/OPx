#include "SymFinder.h"

#include "nodes/IdNode.h"
#include "nodes/DotNode.h"

#include "symbols/Sym.h"

namespace
{

void findIn(SymFinder::Policy policy, Sym *start, Sym *scope, const std::string &name, std::vector<bool> &access, std::vector<SymFinder::Result> &result)
{
    for(auto s: scope->children())
    {
        if(s->type() == Sym::Type::UsingScope && policy != SymFinder::Policy::Declaration)
        {
            auto a = s->accessibleBy(start);
            if(!a)
            {
                access.push_back(false);
            }

            findIn(policy, start, s->property("proxy").to<Sym*>(), name, access, result);

            if(!a)
            {
                access.pop_back();
            }
        }

        if(s->name() == name)
        {
            result.push_back({ s->resolved(), access.empty() ? s->accessibleBy(start) : access.back() });
        }
    }
}

void findFirst(SymFinder::Policy policy, Sym *start, Sym *scope, const std::string &name, std::vector<bool> &access, std::vector<SymFinder::Result> &result)
{
    while(scope)
    {
        findIn(policy, start, scope, name, access, result);
        if(!result.empty())
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
        findFirst(policy, start, start, node.name, access, v);
    }
    else
    {
        for(auto s: scopes)
        {
            findIn(policy, start, s.sym, node.name, access, v);
        }
    }
}

void SymFinder::visit(DotNode &node)
{
    if(scopes.empty())
    {
        findFirst(policy, start, start, node.name, access, scopes);
    }
    else
    {
        std::vector<Result> v;
        for(auto s: scopes)
        {
            findIn(policy, start, s.sym, node.name, access, v);
            scopes = v;
        }
    }

    node.child->accept(*this);
}
