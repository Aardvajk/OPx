#include "SymFinder.h"

#include "nodes/IdNode.h"
#include "nodes/CallNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/ThisNode.h"
#include "nodes/DerefNode.h"
#include "nodes/SubscriptNode.h"
#include "nodes/IncDecNode.h"
#include "nodes/OpEqNode.h"

#include "syms/Sym.h"

#include "visitors/SymScopeVisitor.h"

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
                node.parent->setProperty("sym", sc.front());
                sc = { sc.front()->property<::Type*>("type")->sym };
            }
        }
        else
        {
            sc = { sv.result() };
        }

        for(auto s: sc)
        {
            search(Type::Local, s, node.name, r);
        }
    }
    else
    {
        search(type, curr, node.name, r);
        if(!r.empty() && r.front()->type() == Sym::Type::Var)
        {
            node.setProperty("sym", r.front());
        }
    }
}

void SymFinder::visit(CallNode &node)
{
    node.target->accept(*this);
}

void SymFinder::visit(AddrOfNode &node)
{
    node.expr->accept(*this);
}

void SymFinder::visit(DerefNode &node)
{
    node.expr->accept(*this);
}

void SymFinder::visit(SubscriptNode &node)
{
    node.target->accept(*this);
}

void SymFinder::visit(IncDecNode &node)
{
    node.target->accept(*this);
}

void SymFinder::visit(OpEqNode &node)
{
    node.target->accept(*this);
}

void SymFinder::find(Context &c, Type type, Sym *curr, Node *node, std::vector<Sym*> &result)
{
    SymFinder sf(c, type, curr, result);
    node->accept(sf);
}

