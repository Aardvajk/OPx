#include "DeclarationConstructs.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/ClassNode.h"

#include "symbols/Sym.h"

#include "visitors/NameVisitors.h"

#include "compiler/CommonConstructs.h"
#include "compiler/VarConstructs.h"
#include "compiler/TypeConstructs.h"

namespace
{

Sym *declarationSym(Context &c, Sym::Type type, Sym::Attrs attrs, Node *nn)
{
    auto sym = c.search(SymFinder::Policy::Limited, nn);

    if(!sym)
    {
        if(!NameVisitors::isNameSimple(nn))
        {
            throw Error(nn->location(), "not found - ", NameVisitors::prettyName(nn));
        }

        return c.tree.current()->add(new Sym(type, attrs, nn->location(), NameVisitors::prettyName(nn)));
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

    if(!Sym::isPrimaryScope(proxy->type()))
    {
        throw Error(nn->location(), "scope expected - ", proxy->fullname());
    }

    auto s = c.tree.current()->add(new Sym(Sym::Type::UsingScope, attrs, nn->location(), { }));
    s->setProperty("proxy-scope", proxy);
}

void usingAliasConstruct(Context &c, Sym::Attrs attrs, bool get)
{
    auto nn = CommonConstructs::name(c, get);

    if(c.scanner.token().type() == Token::Type::Assign)
    {
        if(!NameVisitors::isNameSimple(nn.get()))
        {
            throw Error(nn->location(), "id expected - ", NameVisitors::prettyName(nn.get()));
        }

        auto name = NameVisitors::lastIdOfName(nn.get());
        c.assertUnique(nn->location(), name);

        auto type = TypeConstructs::type(c, true);

        auto s = c.tree.current()->add(new Sym(Sym::Type::UsingType, attrs, nn->location(), name));
        s->setProperty("proxy-type", c.types.insert(type.get()));
    }
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

void DeclarationConstructs::entity(Context &c, BlockNode *block, Sym::Attrs attrs, bool get)
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

