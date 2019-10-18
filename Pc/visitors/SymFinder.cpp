#include "SymFinder.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/CallNode.h"
#include "nodes/ProxyCallNode.h"
#include "nodes/ConstructNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/DerefNode.h"
#include "nodes/BinaryNode.h"

#include "visitors/SymScopeVisitor.h"
#include "visitors/TypeVisitor.h"

#include "syms/Sym.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

namespace
{

void findIn(Context &c, Sym *scope, IdNode &node, std::vector<Sym*> &result)
{
    for(auto s: scope->children())
    {
        auto on = node.findProperty("opType");
        auto os = s->findProperty("opType");

        if(on && os)
        {
            if(TypeCompare(c).compatible(on.to<Type*>(), os.to<Type*>()))
            {
                result.push_back(s);
            }
        }
        else if(s->name() == node.name)
        {
            result.push_back(s);
        }
    }
}

void findFirst(Context &c, Sym *scope, IdNode &node, std::vector<Sym*> &result)
{
    while(scope)
    {
        findIn(c, scope, node, result);
        if(!result.empty())
        {
            return;
        }

        scope = scope->parent();
    }
}

void search(Context &c, SymFinder::Type type, Sym *scope, IdNode &node, std::vector<Sym*> &result)
{
    type == SymFinder::Type::Global ? findFirst(c, scope, node, result) : findIn(c, scope, node, result);
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
            search(c, Type::Local, s, node, r);
        }
    }
    else
    {
        search(c, type, curr, node, r);
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

void SymFinder::visit(BinaryNode &node)
{
    auto t = TypeVisitor::assertType(c, &node);
    if(!TypeCompare(c).exact(t, c.types.boolType()))
    {
        node.left->accept(*this);
    }
}

void SymFinder::find(Context &c, Type type, Sym *curr, Node *node, std::vector<Sym*> &result)
{
    SymFinder sf(c, type, curr, result);
    node->accept(sf);
}
