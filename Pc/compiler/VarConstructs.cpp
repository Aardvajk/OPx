#include "VarConstructs.h"

#include "application/Context.h"
#include "application/Error.h"

#include "nodes/BlockNode.h"
#include "nodes/VarNode.h"
#include "nodes/FuncNode.h"

#include "types/Type.h"

#include "compiler/Compiler.h"
#include "compiler/CommonConstructs.h"
#include "compiler/TypeConstructs.h"

namespace
{

void args(Context &c, pcx::ptr_vector<Sym> &syms, bool get)
{
    auto tok = c.scanner.match(Token::Type::Id, get);
    c.scanner.match(Token::Type::Colon, true);

    auto sym = new Sym(Sym::Type::Var, { }, tok.location(), tok.text());
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

void VarConstructs::var(Context &c, BlockNode *block, Sym::Attrs attrs, bool get)
{
    auto tok = c.scanner.match(Token::Type::Id, get);

    c.assertUnique(tok.location(), tok.text());

    auto sym = c.tree.current()->add(new Sym(Sym::Type::Var, attrs, tok.location(), tok.text()));

    c.scanner.match(Token::Type::Colon, true);

    auto tn = TypeConstructs::type(c, true);
    sym->setProperty("type", c.types.insert(tn.get()));

    block->nodes.push_back(new VarNode(tok.location(), sym));

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        var(c, block, attrs, true);
        return;
    }

    c.scanner.consume(Token::Type::Semicolon, false);
}

void VarConstructs::func(Context &c, BlockNode *block, Sym::Attrs attrs, bool get)
{
    auto nn = CommonConstructs::name(c, get);

    pcx::ptr_vector<Sym> av;

    c.scanner.consume(Token::Type::LeftParen, false);
    if(c.scanner.token().type() != Token::Type::RightParen)
    {
        args(c, av, false);
    }

    c.scanner.consume(Token::Type::RightParen, false);
    c.scanner.match(Token::Type::Colon, false);

    TypePtr tn(new Type());
    tn->returnType = TypeConstructs::type(c, true).release();

    for(auto &a: av)
    {
        tn->args.push_back(a.property("type").to<const Type*>()->clone());
    }

    auto sym = c.tree.current()->add(new Sym(Sym::Type::Func, attrs, nn->location(), nn->text()));
    for(std::size_t i = 0; i < av.size(); ++i)
    {
        sym->add(av.release(i));
    }

    sym->setProperty("type", c.types.insert(tn.get()));

    if(c.scanner.token().type() == Token::Type::LeftBrace)
    {
        if(sym->property("defined").value<bool>())
        {
            throw Error(nn->location(), "already defined - ", sym->fullname());
        }

        auto n = new FuncNode(nn->location(), sym);
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
