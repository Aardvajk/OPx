#include "Transformer.h"

#include "application/Context.h"
#include "application/Pragmas.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/IdNode.h"
#include "nodes/VarNode.h"
#include "nodes/ClassNode.h"
#include "nodes/FuncNode.h"
#include "nodes/AssignNode.h"
#include "nodes/InitNode.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include "transform/FuncTransformer.h"
#include "transform/InitMapBuilder.h"

namespace
{

void generateInitialisers(Context &c, FuncNode &node, Sym *sym)
{
    std::unordered_map<std::string, NodePtr> map;

    for(auto &i: node.inits)
    {
        InitMapBuilder mb(map, i);
        i->accept(mb);
    }

    auto block = new BlockNode(node.location());
    node.initialisers = block;

    for(auto s: sym->parent()->children())
    {
        if(s->type() == Sym::Type::Var)
        {
            NodePtr n;

            auto i = map.find(s->name());
            if(i != map.end())
            {
                n = i->second;
            }

            if(!n)
            {
                auto t = s->property<const Type*>("type");

                if(t->ref)
                {
                    throw Error(node.location(), "reference ", s->name(), " must be initialised - ", sym->fullname());
                }

                if(!t->primitive())
                {
                    n = new InitNode(node.location(), s->name());
                    n->setProperty("sym", s);
                }
            }

            if(n)
            {
                block->push_back(n);
            }
        }
    }
}

}

Transformer::Transformer(Context &c) : c(c)
{
}

void Transformer::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void Transformer::visit(NamespaceNode &node)
{
    auto g = c.tree.open(node.property<Sym*>("sym"));
    node.body->accept(*this);
}

void Transformer::visit(ClassNode &node)
{
    if(node.body)
    {
        auto g = c.tree.open(node.property<Sym*>("sym"));
        node.body->accept(*this);
    }
}

void Transformer::visit(FuncNode &node)
{
    auto sym = node.property<Sym*>("sym");

    if(sym->getProperty("method").value<bool>())
    {
        auto n = new VarNode(node.location(), new IdNode(node.location(), { }, "this"));
        node.args.insert(node.args.begin(), n);

        auto t = Type::makePrimary(0, sym->parent());
        t.ref = true;

        auto tt = c.types.insert(t);

        auto s = sym->add(new Sym(Sym::Type::Var, node.location(), "this"));
        s->setProperty("type", tt);

        n->setProperty("sym", s);

        if(!sym->getProperty("thistransformed").value<bool>())
        {
            auto type = *(sym->property<const Type*>("type"));

            type.method = true;
            type.args.insert(type.args.begin(), tt);

            sym->setProperty("type", c.types.insert(type));
        }

        sym->setProperty("thistransformed", true);

        if(sym->name() == "new")
        {
            generateInitialisers(c, node, sym);
        }
    }

    if(node.initialisers)
    {
        auto g = c.tree.open(node.property<Sym*>("sym"));

        FuncTransformer ft(c);
        node.initialisers->accept(ft);
    }

    if(node.body)
    {
        auto g = c.tree.open(node.property<Sym*>("sym"));

        FuncTransformer ft(c);
        node.body->accept(ft);
    }
}

void Transformer::visit(PragmaNode &node)
{
    Pragmas::execute(c, node);
}
