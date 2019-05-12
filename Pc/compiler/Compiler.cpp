#include "Compiler.h"

#include "application/Error.h"
#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/DotNode.h"
#include "nodes/ClassNode.h"

#include "symbols/Sym.h"

#include "visitors/SymFinder.h"
#include "visitors/NameVisitors.h"

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

void classConstruct(Context &c, BlockNode *block, bool get)
{
    auto nn = name(c, get);

    auto sym = c.search(nn.get(), c.tree.current());
    if(!sym)
    {
        if(!NameVisitors::isNameSimple(nn.get()))
        {
            throw Error(nn->location(), "not found - ", nn->text());
        }

        c.assertUnique(nn->location(), nn->text());

        sym = c.tree.current()->add(new Sym(Sym::Type::Class, nn->location(), nn->text()));
    }
    else
    {
        assertType(nn->location(), sym->type(), Sym::Type::Class, nn->text());
    }

    auto cl = new ClassNode(nn->location(), sym);
    block->nodes.push_back(cl);

    c.scanner.match(Token::Type::LeftBrace, false);

    auto scope = new BlockNode(nn->location());
    cl->block = scope;

    auto g = c.tree.open(sym);

    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::RightBrace)
    {
        construct(c, scope, false);
    }

    c.scanner.next(true);
}

void usingClassConstruct(Context &c, bool get)
{
    auto nn = name(c, get);
    auto proxy = c.find(nn.get());

    assertType(nn->location(), proxy->type(), Sym::Type::Class, nn->text());

    auto s = c.tree.current()->add(new Sym(Sym::Type::UsingClass, nn->location(), "[using-class]"));
    s->setProperty("proxy", proxy);
}

void usingAliasConstruct(Context &c, bool get)
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

    auto proxy = c.find(an ? an.get() : nn.get());
    auto name = c.assertUnique(nn->location(), NameVisitors::lastIdOfName(nn.get()));

    auto s = c.tree.current()->add(new Sym(Sym::Type::Using, nn->location(), name));
    s->setProperty("proxy", proxy);
}

void usingConstruct(Context &c, bool get)
{
    auto tok = c.scanner.next(get);

    if(tok.type() == Token::Type::RwClass)
    {
        usingClassConstruct(c, true);
    }
    else
    {
        usingAliasConstruct(c, false);
    }

    c.scanner.consume(Token::Type::Semicolon, false);
}

void idTest(Context &c, bool get)
{
    auto nn = name(c, get);

    SymFinder sf(c.tree.current(), nullptr);
    nn->accept(sf);

    auto r = sf.result();
    if(!r.empty())
    {
        std::cout << "searching with " << nn->text() << "\n";
        for(auto i: r)
        {
            std::cout << "    " << i->fullname() << " [" << i << "]\n";
        }
    }
    else
    {
        std::cout << "using " << nn->text() << " nothing found\n";
    }

    c.scanner.consume(Token::Type::Semicolon, false);
}

void construct(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwClass: classConstruct(c, block, true); break;
        case Token::Type::RwUsing: usingConstruct(c, true); break;

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
