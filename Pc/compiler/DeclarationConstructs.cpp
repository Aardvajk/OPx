#include "DeclarationConstructs.h"

#include "application/Error.h"
#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/ClassNode.h"

#include "symbols/Sym.h"

#include "visitors/NameVisitors.h"

#include "compiler/CommonConstructs.h"
#include "compiler/VarConstructs.h"

namespace
{

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

    if(sym->type() != type)
    {
        throw Error(nn->location(), Sym::toString(type), " expected - ", sym->fullname());
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
    n->block = CommonConstructs::scopeContents(c, nn->location(), false);
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
        n->block = CommonConstructs::scopeContents(c, nn->location(), false);

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

}

void DeclarationConstructs::declaration(Context &c, BlockNode *block, Sym::Attrs attrs, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwNamespace: namespaceConstruct(c, block, attrs, true); break;
        case Token::Type::RwClass: classConstruct(c, block, attrs, true); break;

        case Token::Type::RwUsing: usingConstruct(c, attrs, true); break;

        case Token::Type::RwVar: VarConstructs::var(c, block, attrs, true); break;
        case Token::Type::RwFunc: VarConstructs::func(c, block, attrs, true); break;

        default: throw Error(tok.location(), "declaration expected - ", tok.text());
    }
}

