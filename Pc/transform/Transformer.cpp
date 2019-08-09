#include "Transformer.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/IdNode.h"
#include "nodes/VarNode.h"
#include "nodes/ClassNode.h"
#include "nodes/FuncNode.h"

#include "types/Type.h"

#include "transform/FuncTransformer.h"

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

        auto tt = c.types.insert(Type::makePrimary(1, sym->parent()));

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
    }

    if(node.body)
    {
        auto g = c.tree.open(node.property<Sym*>("sym"));

        FuncTransformer ft(c);
        node.body->accept(ft);
    }
}
