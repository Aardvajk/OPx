#include "SymFinder.h"

#include "nodes/GlobalNode.h"
#include "nodes/IdNode.h"
#include "nodes/DotNode.h"

#include "symbols/Sym.h"

#include "types/Type.h"

namespace
{

void findIn(SymFinder::Policy policy, Sym *start, Sym *scope, const std::string &name, std::vector<bool> &access, std::vector<SymResult> &result)
{
    for(auto s: scope->children())
    {
        if(s->name() == name)
        {
            result.push_back({ s, s->accessibleBy(start) });
        }
    }
}

void findFirst(SymFinder::Policy policy, Sym *start, Sym *scope, const std::string &name, std::vector<bool> &access, std::vector<SymResult> &result)
{
    while(scope)
    {
        findIn(policy, start, scope, name, access, result);
        if(!result.empty())
        {
            return;
        }

        if(policy == SymFinder::Policy::Limited && Sym::isPrimaryScope(scope->type()))
        {
            return;
        }

        scope = scope->parent();
    }
}

}

SymFinder::SymFinder(Policy policy, Sym *root, Sym *start) : policy(policy), root(root), start(start)
{
}

void SymFinder::visit(GlobalNode &node)
{
    scopes.push_back({ root, true });
    policy = Policy::Limited;

    node.child->accept(*this);
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

    node.setProperty("found", v);
}

void SymFinder::visit(DotNode &node)
{
    if(scopes.empty())
    {
        findFirst(policy, start, start, node.name, access, scopes);
    }
    else
    {
        std::vector<SymResult> v;
        for(auto s: scopes)
        {
            findIn(policy, start, s.sym, node.name, access, v);
            scopes = v;
        }
    }

    node.setProperty("found", scopes);

    if(scopes.size() == 1)
    {
        if(scopes.front().sym->type() == Sym::Type::Var)
        {
            auto type = scopes.front().sym->property("type").to<const Type*>();
            if(type->isClass())
            {
                scopes.clear();
                scopes.push_back({ type->sym, true });
            }
        }
    }

    if(!scopes.empty())
    {
        node.child->accept(*this);
    }
}
