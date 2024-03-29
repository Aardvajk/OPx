#include "Decorator.h"

#include "framework/Error.h"

#include "common/Primitive.h"

#include "application/Context.h"
#include "application/Pragmas.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"
#include "nodes/IdNode.h"
#include "nodes/ExprNode.h"
#include "nodes/ProxyCallNode.h"

#include "visitors/SymFinder.h"
#include "visitors/NameVisitors.h"
#include "visitors/TypeVisitor.h"
#include "visitors/QueryVisitors.h"

#include "types/Type.h"
#include "types/TypeCompare.h"
#include "types/TypeBuilder.h"
#include "types/TypeLookup.h"

#include "decorator/TypeDecorator.h"
#include "decorator/VarDecorator.h"
#include "decorator/FuncDecorator.h"
#include "decorator/DefaultMethods.h"
#include "decorator/ArgDecorator.h"

#include "generator/ByteListGenerator.h"

#include <pcx/scoped_counter.h>
#include <pcx/scoped_push.h>

namespace
{

Sym *search(Context &c, Sym::Type type, Node *node)
{
    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Local, c.tree.current(), node, sv);

    for(auto s: sv)
    {
        if(s->type() != type)
        {
            throw Error(node->location(), Sym::toString(type), " expected - ", s->fullname());
        }

        return s;
    }

    return nullptr;
}

Sym *searchFunc(Context &c, Node *node, const Type *type)
{
    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Local, c.tree.current(), node, sv);

    for(auto s: sv)
    {
        if(s->type() != Sym::Type::Func)
        {
            throw Error(node->location(), "function expected - ", s->fullname());
        }

        auto t = s->property<Type*>("type");
        if(TypeCompare(c).exactArgs(type, t) && type->constMethod == t->constMethod)
        {
            return s;
        }
    }

    return nullptr;
}

void decorateFunctionBody(Context &c, FuncNode &node, Sym *sym)
{
    if(sym->findProperty("defined").value<bool>() && !c.instantiating)
    {
        throw Error(node.location(), "already defined - ", sym->fullname());
    }

    sym->setProperty("defined", true);

    c.funcInfos.push_back(new FuncInfo());
    sym->setProperty("info", c.funcInfos.back_ptr());

    auto sg = c.tree.open(sym);

    for(auto &a: node.args)
    {
        Visitor::visit<VarDecorator>(a.get(), c);
    }

    for(auto &i: node.inits)
    {
        Visitor::visit<FuncDecorator>(i.get(), c);
    }

    Visitor::visit<FuncDecorator>(node.body.get(), c);

    auto t = sym->property<Type*>("type")->returnType;
    if(!TypeCompare(c).compatible(t, c.types.nullType()) && !sym->findProperty("returned").value<bool>())
    {
        throw Error(node.location(), "function must return ", t->text());
    }
}

}

Decorator::Decorator(Context &c, NodePtr &cn) : c(c), cn(cn)
{
}

void Decorator::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        Decorator::decorate(c, n);
    }
}

void Decorator::visit(NamespaceNode &node)
{
    auto sym = search(c, Sym::Type::Namespace, node.name.get());
    if(!sym)
    {
        auto n = NameVisitors::assertSimpleUniqueName(c, node.name.get());
        sym = c.tree.current()->add(new Sym(Sym::Type::Namespace, node.name->location(), node.property<Access>("access"), n));
    }

    node.setProperty("sym", sym);

    auto sg = c.tree.open(sym);
    Decorator::decorate(c, node.body);
}

void Decorator::visit(FuncNode &node)
{
    auto gp = pcx::scoped_push(c.generics, node.generics);

    std::size_t defaults = 0;
    for(auto &a: node.args)
    {
        Visitor::visit<ArgDecorator>(a.get(), c, defaults);
    }

    auto opType = Visitor::query<NameVisitors::ResolveOpName, Type*>(node.name.get(), c);

    if(node.type)
    {
        if(opType)
        {
            throw Error(node.type->location(), "cannot explicitly return - ", node.description());
        }

        Visitor::visit<TypeDecorator>(node.type.get(), c);
    }

    auto t = Type::makeFunction(node.type ? TypeVisitor::assertType(c, node.type.get()) : (opType ? opType : c.types.nullType()));

    t.method = c.tree.current()->type() == Sym::Type::Class && !node.findProperty("free").value<bool>();
    t.constMethod = node.constMethod;

    if(opType)
    {
        if((t.method && !node.args.empty()) || (!t.method && node.args.size() != 1))
        {
            throw Error(node.name->location(), "invalid conversion operator - ", node.name->description());
        }
    }

    for(auto &a: node.args)
    {
        t.args.push_back(TypeVisitor::assertType(c, a.get()));
    }

    auto type = c.types.insert(t);

    auto sym = searchFunc(c, node.name.get(), type);
    if(sym)
    {
        if(!TypeCompare(c).exact(sym->property<Type*>("type")->returnType, type->returnType))
        {
            throw Error(node.location(), "mismatched return type - ", node.name->description());
        }

        if(auto gp = sym->findProperty("generics"))
        {
            if(!gp.to<GenericParams>().namesEquivalent(node.generics))
            {
                throw Error(node.location(), "mismatched generics - ", node.name->description());
            }
        }

        if(sym->findProperty("defaults") && defaults)
        {
            throw Error(node.location(), "defaults already declared - ", node.description());
        }

        if(node.body)
        {
            auto old = sym->findProperty("funcnode").value<FuncNode*>();

            sym->setProperty("funcnode", &node);
            if(old && sym->findProperty("defaults"))
            {
                for(std::size_t i = 0; i < node.args.size(); ++i)
                {
                    node.args[i] = old->args[i];
                }
            }
        }

        if(defaults)
        {
            sym->setProperty("defaults", defaults);
        }
    }
    else
    {
        auto n = NameVisitors::assertSimpleName(c, node.name.get());

        sym = c.tree.current()->add(new Sym(Sym::Type::Func, node.name->location(), node.property<Access>("access"), n));

        if(node.findProperty("explicit").value<bool>() && n != "new" && !opType)
        {
            throw Error(node.location(), "cannot be explicit - ", node.name->description());
        }

        sym->setProperty("type", type);
        sym->setProperty("autogen", node.findProperty("autogen").value<bool>());
        sym->setProperty("method", t.method);
        sym->setProperty("explicit", node.findProperty("explicit").value<bool>());
        sym->setProperty("funcnode", &node);

        if(node.generics)
        {
            sym->setProperty("generics", node.generics);
        }

        if(defaults)
        {
            sym->setProperty("defaults", defaults);
        }

        if(opType)
        {
            sym->setProperty("opType", opType);
        }
    }

    if(sym->parent()->type() != Sym::Type::Class && Visitor::query<NameVisitors::SpecialName, Token::Type>(node.name.get()) != Token::Type::Invalid)
    {
        throw Error(node.name->location(), "invalid function name - ", node.name->description());
    }

    if(t.constMethod && sym->parent()->type() != Sym::Type::Class)
    {
        throw Error(node.location(), "cannot be const - ", node.name->description());
    }

    node.setProperty("sym", sym);

    if(node.body && !node.generics)
    {
        if(c.tree.current()->container()->type() == Sym::Type::Class && c.classDepth)
        {
            c.deferredMethods.push_back(&node);
        }
        else
        {
            decorateFunctionBody(c, node, sym);
        }
    }
}

void Decorator::visit(ClassNode &node)
{
    auto sym = search(c, Sym::Type::Class, node.name.get());
    if(!sym)
    {
        auto n = NameVisitors::assertSimpleUniqueName(c, node.name.get());
        sym = c.tree.current()->add(new Sym(Sym::Type::Class, node.name->location(), node.property<Access>("access"), n));

        sym->setProperty("type", c.types.insert(Type::makePrimary(sym)));
        sym->setProperty("primitive", Primitive::Type::Invalid);
    }

    node.setProperty("sym", sym);

    if(node.body)
    {
        if(sym->findProperty("defined").value<bool>())
        {
            throw Error(node.location(), "already defined - ", sym->fullname());
        }

        sym->setProperty("defined", true);

        auto sg = c.tree.open(sym);
        auto dg = pcx::scoped_counter(c.classDepth);

        Decorator::decorate(c, node.body);

        DefaultMethods::generate(c, node, sym);

        auto type = sym->property<Type*>("type");

        sym->setProperty("cachedCopyMethod", TypeLookup::findCopyMethod(c, type));
        sym->setProperty("cachedDeleteMethod", TypeLookup::findDeleteMethod(c, type));
    }

    if(!c.classDepth)
    {
        for(auto d: c.deferredMethods)
        {
            decorateFunctionBody(c, *d, d->property<Sym*>("sym"));
        }

        c.deferredMethods.clear();
    }
}

void Decorator::visit(VarNode &node)
{
    Visitor::visit<VarDecorator>(&node, c);

    auto sym = node.property<Sym*>("sym");
    auto type = sym->property<Type*>("type");

    if(!sym->findProperty("member").value<bool>() && !node.findProperty("external").value<bool>())
    {
        if(!type->primitive() || type->ref || (node.value && !Visitor::query<CanByteListGenerate, bool>(node.value.get())))
        {
            auto block = Visitor::query<QueryVisitors::GetBlockNode, BlockNode*>(c.globalInit);
            block->push_back(cn);

            node.setProperty("globalinit", true);
        }

        if(!type->primitiveOrRef())
        {
            auto block = Visitor::query<QueryVisitors::GetBlockNode, BlockNode*>(c.globalDestroy);

            auto en = new ExprNode(node.location());
            block->insert(0, en);

            NodePtr id(IdNode::create(node.location(), sym->names()));

            auto dm = TypeLookup::assertDeleteMethod(c, node.location(), type);

            auto cn = new ProxyCallNode(node.location(), dm, id);
            en->expr = cn;
        }
    }
}

void Decorator::visit(PragmaNode &node)
{
    Pragmas::execute(c, node);
}

void Decorator::decorate(Context &c, NodePtr node)
{
    Decorator d(c, node);
    node->accept(d);
}

void Decorator::decorateFunction(Context &c, FuncNode *node)
{
    decorateFunctionBody(c, *node, node->property<Sym*>("sym"));
}
