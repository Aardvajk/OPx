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

void search(SymFinder::Type type, Sym *scope, const std::string &name, std::vector<Sym*> &result)
{
    type == SymFinder::Type::Global ? findFirst(scope, name, result) : findIn(scope, name, result);
}

}

SymFinder::SymFinder(Type type, Sym *curr) : type(type), curr({ curr })
{
}

void SymFinder::visit(IdNode &node)
{
    search(type, curr.back(), node.name, r);
    type = Type::Local;
}

void SymFinder::visit(DotNode &node)
{
    std::vector<Sym*> scopes;

    search(type, curr.back(), node.name, scopes);
    type = Type::Local;

    for(auto scope: scopes)
    {
        auto g = pcx::scoped_push(curr, scope);
        node.child->accept(*this);
    }
}

std::vector<Sym*> SymFinder::find(Type type, Sym *curr, Node *node)
{
    SymFinder sf(type, curr);
    node->accept(sf);

    return sf.result();
}

