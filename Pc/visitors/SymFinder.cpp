#include "SymFinder.h"

#include "nodes/IdNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

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

SymFinder::SymFinder(Type type, Sym *curr, std::vector<Sym*> &result) : type(type), curr({ curr }), r(result)
{
}

void SymFinder::visit(IdNode &node)
{
    if(node.parent)
    {
        std::vector<Sym*> sc;
        SymFinder::find(type, curr.back(), node.parent.get(), sc);

        for(auto s: sc)
        {
            search(type, s, node.name, r);
        }
    }
    else
    {
        search(type, curr.back(), node.name, r);
    }
}

void SymFinder::find(Type type, Sym *curr, Node *node, std::vector<Sym*> &result)
{
    SymFinder sf(type, curr, result);
    node->accept(sf);
}

