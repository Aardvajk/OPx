#include "Compiler.h"

#include "application/Error.h"
#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/DotNode.h"
#include "nodes/ClassNode.h"

#include "symbols/Sym.h"

#include "visitors/SymFinder.h"
#include "visitors/IdResolver.h"

namespace
{

void construct(Context &c, BlockNode *block, bool get);

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
    c.scanner.match(Token::Type::RwClass, get);
    auto nn = name(c, true);

    auto sym = c.tree.current()->add(new Sym(Sym::Type::Class, nn->location(), nn->text()));

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

void usingConstruct(Context &c, bool get)
{
    c.scanner.match(Token::Type::RwUsing, get);
    c.scanner.match(Token::Type::RwClass, true);

    auto nn = name(c, true);
    SymFinder sf(c.tree.current());
    nn->accept(sf);

    if(sf.result().empty())
    {
        throw Error(nn->location(), "not found - ", nn->text());
    }

    if(sf.result().size() > 1)
    {
        throw Error(nn->location(), "ambiguous - ", nn->text());
    }

    auto proxy = sf.result().front();
    if(proxy->type() != Sym::Type::Class)
    {
        throw Error(nn->location(), "invalid using class - ", nn->text);
    }

    auto s = c.tree.current()->add(new Sym(Sym::Type::UsingClass, nn->location(), "[using-scope]"));
    s->setProperty("proxy", proxy);

    c.scanner.consume(Token::Type::Semicolon, false);
}

void idTest(Context &c, bool get)
{
    auto nn = name(c, get);

    SymFinder sf(c.tree.current());
    nn->accept(sf);
    if(!sf.result().empty())
    {
        std::cout << "searching with " << nn->text() << "\n";
        for(auto i: sf.result())
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
        case Token::Type::RwClass: classConstruct(c, block, false); break;
        case Token::Type::RwUsing: usingConstruct(c, false); break;

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
