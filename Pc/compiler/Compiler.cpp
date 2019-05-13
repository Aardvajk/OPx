#include "Compiler.h"

#include "application/Error.h"
#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/ClassNode.h"
#include "nodes/IdNode.h"
#include "nodes/DotNode.h"

#include "symbols/Sym.h"

#include "visitors/SymFinder.h"
#include "visitors/NameVisitors.h"

#include <pcx/scoped_push.h>

namespace
{

void construct(Context &c, BlockNode *block, bool get);

void assertType(Location location, Sym::Type type, Sym::Type expected, const std::string &text)
{
    if(type != expected)
    {
        throw Error(location, Sym::toString(expected), " expected - ", text);
    }
}

Sym::Attrs defaultAttrs(Sym::Type type)
{
    switch(type)
    {
        case Sym::Type::Namespace: return Sym::Attr::Public;
        case Sym::Type::Class: return Sym::Attr::Private;

        default: return { };
    }
}

NodePtr name(Context &c, bool get)
{
    auto tok = c.scanner.match(Token::Type::Id, get);

    auto dot = c.scanner.next(true);
    if(dot.type() == Token::Type::Dot)
    {
        return new DotNode(dot.location(), tok.text(), name(c, true));
    }

    return new IdNode(tok.location(), tok.text());
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

void namespaceConstruct(Context &c, BlockNode *block, Sym::Attrs attrs, bool get)
{
    auto nn = name(c, get);

    auto sym = c.search(SymFinder::Policy::Declaration, nn.get());

    if(!sym)
    {
        if(!NameVisitors::isNameSimple(nn.get()))
        {
            throw Error(nn->location(), "not found - ", nn->text());
        }

        sym = c.tree.current()->add(new Sym(Sym::Type::Namespace, attrs, nn->location(), nn->text()));
    }

    auto cl = new NamespaceNode(nn->location(), sym);
    block->nodes.push_back(cl);

    auto g = c.tree.open(sym);
    cl->block = scopeContents(c, nn->location(), false);
}

void classConstruct(Context &c, BlockNode *block, Sym::Attrs attrs, bool get)
{
    auto tok = c.scanner.match(Token::Type::Id, get);

    c.assertUnique(tok.location(), tok.text());

    auto sym = c.tree.current()->add(new Sym(Sym::Type::Class, attrs, tok.location(), tok.text()));

    auto cl = new ClassNode(tok.location(), sym);
    block->nodes.push_back(cl);

    auto g = c.tree.open(sym);
    cl->block = scopeContents(c, tok.location(), true);
}

void usingScopeConstruct(Context &c, Sym::Attrs attrs, Sym::Type type, bool get)
{
    auto nn = name(c, get);
    auto proxy = c.find(SymFinder::Policy::Symbol, nn.get());

    assertType(nn->location(), proxy->type(), type, nn->text());

    auto s = c.tree.current()->add(new Sym(Sym::Type::UsingScope, attrs, nn->location(), "[using-scope]"));
    s->setProperty("proxy", proxy);
}

void usingAliasConstruct(Context &c, Sym::Attrs attrs, bool get)
{
    auto nn = name(c, get);

    NodePtr an;
    if(c.scanner.token().type() == Token::Type::Assign)
    {
        if(!NameVisitors::isNameSimple(nn.get()))
        {
            throw Error(nn->location(), "id expected - ", nn->text());
        }

        an = name(c, true);
    }

    auto proxy = c.find(SymFinder::Policy::Symbol, an ? an.get() : nn.get());
    auto name = c.assertUnique(nn->location(), NameVisitors::lastIdOfName(nn.get()));

    auto s = c.tree.current()->add(new Sym(Sym::Type::Using, attrs, nn->location(), name));
    s->setProperty("proxy", proxy);
}

void usingConstruct(Context &c, Sym::Attrs attrs, bool get)
{
    auto tok = c.scanner.next(get);

    switch(tok.type())
    {
        case Token::Type::RwClass: usingScopeConstruct(c, attrs, Sym::Type::Class, true); break;
        case Token::Type::RwNamespace: usingScopeConstruct(c, attrs, Sym::Type::Namespace, true); break;

        default: usingAliasConstruct(c, attrs, false);
    }

    c.scanner.consume(Token::Type::Semicolon, false);
}

void idTest(Context &c, bool get)
{
    auto nn = name(c, get);

    c.scanner.consume(Token::Type::Semicolon, false);

    SymFinder sf(SymFinder::Policy::Symbol, c.tree.current());
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

        case Token::Type::Id: idTest(c, false); break;

        default: throw Error(tok.location(), "construct expected - ", tok.text());
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
