#include "Generator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"
#include "nodes/FuncNode.h"

#include "visitors/NameVisitors.h"
#include "visitors/SymFinder.h"

#include "types/Type.h"
#include "types/TypeBuilder.h"

#include <pcx/scoped_counter.h>

Generator::Generator(Context &c, std::ostream &os) : c(c), os(os), classDepth(0)
{
}

void Generator::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n.accept(*this);
    }
}

void Generator::visit(NamespaceNode &node)
{
    auto name = NameVisitors::prettyName(node.name.get());
    if(!NameVisitors::isNameSimple(node.name.get()))
    {
        throw Error(node.location(), "simple name expected - ", name);
    }

    c.assertUnique(node.name->location(), name);

    auto sym = c.tree.current()->add(new Sym(Sym::Type::Namespace, node.name->location(), name));

    auto g = c.tree.open(sym);
    node.block->accept(*this);
}

void Generator::visit(ClassNode &node)
{
    if(node.block)
    {
        auto g = pcx::scoped_counter(classDepth);
        node.block->accept(*this);
    }
}

void Generator::visit(VarNode &node)
{
}

void Generator::visit(FuncNode &node)
{
    Type t;
    t.returnType = node.type ? TypeBuilder::type(c, node.type.get()) : c.types.nullType();

    for(auto &a: node.args)
    {
        t.args.push_back(TypeBuilder::type(c, &a));
    }
}
