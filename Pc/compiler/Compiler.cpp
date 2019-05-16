#include "Compiler.h"

#include "application/Error.h"
#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/ClassNode.h"
#include "nodes/GlobalNode.h"
#include "nodes/IdNode.h"
#include "nodes/DotNode.h"

#include "symbols/Sym.h"

#include "visitors/SymFinder.h"
#include "visitors/NameVisitors.h"

#include "compiler/CommonConstructs.h"
#include "compiler/TestConstructs.h"

#include <pcx/scoped_push.h>

namespace
{

void construct(Context &c, BlockNode *block, bool get);

Sym::Attrs defaultAttrs(Sym::Type type)
{
    switch(type)
    {
        case Sym::Type::Namespace: return Sym::Attr::Public;
        case Sym::Type::Class: return Sym::Attr::Private;

        default: return { };
    }
}

NodePtr scopeContents(Context &c, Location location, bool get)
{
    auto block = new BlockNode(location);
    NodePtr nn(block);

    c.scanner.match(Token::Type::LeftBrace, get);

    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::RightBrace)
    {
        construct(c, block, false);
    }

    c.scanner.next(true);

    return nn;
}

Sym *declarationSym(Context &c, Sym::Type type, Sym::Attrs attrs, Node *nn)
{
    auto sym = c.search(SymFinder::Policy::Limited, nn);

    if(!sym)
    {
        if(!NameVisitors::isNameSimple(nn))
        {
            throw Error(nn->location(), "not found - ", nn->text());
        }

        return c.tree.current()->add(new Sym(type, attrs, nn->location(), nn->text()));
    }

    return sym;
}

void namespaceConstruct(Context &c, BlockNode *block, Sym::Attrs attrs, bool get)
{
    auto nn = CommonConstructs::name(c, get);
    auto sym = declarationSym(c, Sym::Type::Namespace, attrs, nn.get());

    auto n = new NamespaceNode(nn->location(), sym);
    block->nodes.push_back(n);

    auto g = c.tree.open(sym);
    n->block = scopeContents(c, nn->location(), false);
}

void classConstruct(Context &c, BlockNode *block, Sym::Attrs attrs, bool get)
{
    auto nn = CommonConstructs::name(c, get);
    auto sym = declarationSym(c, Sym::Type::Class, attrs, nn.get());

    if(c.scanner.token().type() == Token::Type::LeftBrace)
    {
        if(sym->property("defined").value<bool>())
        {
            throw Error(nn->location(), "already defined - ", sym->fullname());
        }

        auto n = new ClassNode(nn->location(), sym);
        block->nodes.push_back(n);

        auto g = c.tree.open(sym);
        n->block = scopeContents(c, nn->location(), false);

        sym->setProperty("defined", true);
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }
}

void usingScopeConstruct(Context &c, Sym::Attrs attrs, bool get)
{
    auto nn = CommonConstructs::name(c, get);
    auto proxy = c.find(SymFinder::Policy::Full, nn.get());

    if(!Sym::isImportableScope(proxy->type()))
    {
        throw Error(nn->location(), "scope expected - ", proxy->fullname());
    }

    auto s = c.tree.current()->add(new Sym(Sym::Type::UsingScope, attrs, nn->location(), { }));
    s->setProperty("proxy", proxy);
}

void usingAliasConstruct(Context &c, Sym::Attrs attrs, bool get)
{
    auto nn = CommonConstructs::name(c, get);

    NodePtr an;
    if(c.scanner.token().type() == Token::Type::Assign)
    {
        if(!NameVisitors::isNameSimple(nn.get()))
        {
            throw Error(nn->location(), "id expected - ", nn->text());
        }

        an = CommonConstructs::name(c, true);
    }

    auto proxy = c.find(SymFinder::Policy::Full, an ? an.get() : nn.get());
    auto name = c.assertUnique(nn->location(), NameVisitors::lastIdOfName(nn.get()));

    auto s = c.tree.current()->add(new Sym(Sym::Type::Using, attrs, nn->location(), name));
    s->setProperty("proxy", proxy);
}

void usingConstruct(Context &c, Sym::Attrs attrs, bool get)
{
    auto tok = c.scanner.next(get);

    switch(tok.type())
    {
        case Token::Type::RwNamespace: usingScopeConstruct(c, attrs, true); break;

        default: usingAliasConstruct(c, attrs, false);
    }

    c.scanner.consume(Token::Type::Semicolon, false);
}

void idTest(Context &c, bool get)
{
    auto nn = CommonConstructs::name(c, get);

    c.scanner.consume(Token::Type::Semicolon, false);

    SymFinder sf(SymFinder::Policy::Full, c.tree.root(), c.tree.current());
    nn->accept(sf);

    auto r = sf.result();
    if(!r.empty())
    {
        std::cout << "searching with " << nn->text() << "\n";
        for(auto i: r)
        {
            std::cout << "    " << i.sym->fullname() << " [" << i.sym << "] access [" << i.accessible << "]\n";
        }
    }
    else
    {
        std::cout << "using " << nn->text() << " nothing found\n";
    }
}

void declarationConstruct(Context &c, BlockNode *block, Sym::Attrs attrs, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwNamespace: namespaceConstruct(c, block, attrs, true); break;
        case Token::Type::RwClass: classConstruct(c, block, attrs, true); break;

        case Token::Type::RwUsing: usingConstruct(c, attrs, true); break;

        default: throw Error(tok.location(), "declaration expected - ", tok.text());
    }
}

void construct(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwNamespace:
        case Token::Type::RwClass:
        case Token::Type::RwUsing: declarationConstruct(c, block, defaultAttrs(c.tree.current()->type()), false); break;

        case Token::Type::RwPublic: declarationConstruct(c, block, Sym::Attr::Public, true); break;
        case Token::Type::RwPrivate: declarationConstruct(c, block, Sym::Attr::Private, true); break;

        case Token::Type::RwLookup: TestConstructs::lookup(c, true); break;

        default: idTest(c, false);
    }
}

}

NodePtr compile(Context &c)
{
    auto block = new BlockNode(Location());
    NodePtr n(block);

    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::Eof)
    {
        construct(c, block, false);
    }

    return n;
}
