#include "SymFinder.h"

#include "nodes/IdNode.h"
#include "nodes/DotNode.h"

#include "syms/Sym.h"

#include <pcx/scoped_push.h>

namespace
{

void findIn(Sym *scope, const std::string &name, std::vector<Sym*> &result)
{
    for(auto s: scope->children())
    {
        if(s->name() == name)
        {
            result.push_back(s);
        }
    }
}

void findFirst(Sym *scope, const std::string &name, std::vector<Sym*> &result)
{
    while(scope)
    {
        findIn(scope, name, result);
        if(!result.empty())
        {
            return;
        }

        scope = scope->parent();
    }
}

}

SymFinder::SymFinder(Sym *curr) : curr({ curr })
{
}

void SymFinder::visit(IdNode &node)
{
    findFirst(curr.back(), node.name, r);
}

void SymFinder::visit(DotNode &node)
{
    std::vector<Sym*> scopes;
    findFirst(curr.back(), node.name, scopes);

    for(auto scope: scopes)
    {
        auto g = pcx::scoped_push(curr, scope);
        node.child->accept(*this);
    }
}

std::vector<Sym*> SymFinder::find(Sym *curr, Node *node)
{
    SymFinder sf(curr);
    node->accept(sf);

    return sf.result();
}

