#include "SymFinder.h"

#include "nodes/IdNode.h"
#include "nodes/CallNode.h"
#include "nodes/ProxyCallNode.h"
#include "nodes/ConstructNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/DerefNode.h"

#include "visitors/SymScopeVisitor.h"

#include "syms/Sym.h"

#include "types/Type.h"

namespace
{

void findIn(Sym *scope, IdNode &node, std::vector<Sym*> &result)
{
    for(auto s: scope->children())
    {
        if(s->name() == node.name)
        {
            result.push_back(s);
        }
    }
}

void findFirst(Sym *scope, IdNode &node, std::vector<Sym*> &result)
{
    while(scope)
    {
        findIn(scope, node, result);
        if(!result.empty())
        {
            return;
        }

        scope = scope->parent();
    }
}

void search(SymFinder::Type type, Sym *scope, IdNode &node, std::vector<Sym*> &result)
{
    type == SymFinder::Type::Global ? findFirst(scope, node, result) : findIn(scope, node, result);
}

}

SymFinder::SymFinder(Context &c, Type type, Sym *curr, std::vector<Sym*> &result) : c(c), type(type), curr(curr), r(result)
{
}

void SymFinder::visit(IdNode &node)
{
    if(node.parent)
    {
        SymScopeVisitor sv(c, curr);
        node.parent->accept(sv);

        std::vector<Sym*> sc;
        if(sv.result() == curr)
        {
            SymFinder::find(c, type, curr, node.parent.get(), sc);

            if(!sc.empty() && sc.front()->type() == Sym::Type::Var)
            {
                sc = { sc.front()->property<::Type*>("type")->sym };
            }
        }
        else
        {
            sc = { sv.result() };
        }

        for(auto s: sc)
        {
            search(Type::Local, s, node, r);
        }
    }
    else
    {
        search(type, curr, node, r);
    }
}

void SymFinder::visit(AddrOfNode &node)
{
    node.expr->accept(*this);
}

void SymFinder::visit(DerefNode &node)
{
    node.expr->accept(*this);
}

void SymFinder::find(Context &c, Type type, Sym *curr, Node *node, std::vector<Sym*> &result)
{
    SymFinder sf(c, type, curr, result);
    node->accept(sf);
}
