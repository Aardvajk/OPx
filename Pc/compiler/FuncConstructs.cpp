#include "FuncConstructs.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "compiler/CommonConstructs.h"
#include "compiler/TypeConstructs.h"

#include "visitors/NameVisitors.h"

#include "types/TypeCompare.h"

#include "nodes/BlockNode.h"
#include "nodes/FuncNode.h"

namespace
{

void args(Context &c, pcx::ptr_vector<Sym> &syms, bool get)
{
    Token name;

    auto tok = c.scanner.next(get);
    if(tok.type() == Token::Type::Id)
    {
        name = tok;
        c.scanner.next(true);
    }

    c.scanner.match(Token::Type::Colon, false);

    auto sym = new Sym(Sym::Type::Var, { }, tok.location(), name.text());
    syms.push_back(sym);

    sym->setProperty("argument", true);

    auto tn = TypeConstructs::type(c, true);
    sym->setProperty("type", c.types.insert(tn.get()));

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        args(c, syms, true);
    }
}

}

void FuncConstructs::func(Context &c, BlockNode *block, Sym::Attrs attrs, bool get)
{
    auto nn = CommonConstructs::name(c, get);

    pcx::ptr_vector<Sym> av;

    c.scanner.consume(Token::Type::LeftParen, false);
    if(c.scanner.token().type() != Token::Type::RightParen)
    {
        args(c, av, false);
    }

    c.scanner.consume(Token::Type::RightParen, false);

    TypePtr tn(new Type());
    if(c.scanner.token().type() == Token::Type::Colon)
    {
        tn->returnType = TypeConstructs::type(c, true).release();
    }
    else
    {
        tn->returnType = new Type(c.tree.root()->child("std")->child("null"));
    }

    for(auto &a: av)
    {
        tn->args.push_back(a.property("type").to<const Type*>()->clone());
    }

    auto sym = c.matchFunction(SymFinder::Policy::Limited, nn.get(), tn.get());
    if(!sym)
    {
        if(!NameVisitors::isNameSimple(nn.get()))
        {
            throw Error(nn->location(), "not found - ", NameVisitors::prettyName(nn.get()));
        }

        sym = c.tree.current()->add(new Sym(Sym::Type::Func, attrs, nn->location(), NameVisitors::prettyName(nn.get())));
        sym->setProperty("type", c.types.insert(tn.get()));
    }
    else
    {
        if(!TypeCompare::exact(sym->property("type").to<const Type*>()->returnType.get(), tn->returnType.get()))
        {
            throw Error(nn->location(), "mismatched return types - ", sym->fullname());
        }
    }

    if(c.scanner.token().type() == Token::Type::LeftBrace)
    {
        if(sym->property("defined").value<bool>())
        {
            throw Error(nn->location(), "already defined - ", sym->fullname());
        }

        auto n = new FuncNode(nn->location(), sym);
        block->nodes.push_back(n);

        auto g = c.tree.open(sym);

        for(std::size_t i = 0; i < av.size(); ++i)
        {
            sym->add(av.release(i));
        }

        n->block = CommonConstructs::scopeContents(c, nn->location(), false);

        sym->setProperty("defined", true);
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }
}
