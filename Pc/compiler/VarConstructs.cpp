#include "VarConstructs.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/VarNode.h"

#include "compiler/TypeConstructs.h"
#include "compiler/Expr.h"

void VarConstructs::var(Context &c, BlockNode *block, Sym::Attrs attrs, bool get)
{
    auto tok = c.scanner.match(Token::Type::Id, get);

    c.assertUnique(tok.location(), tok.text());

    auto sym = c.tree.current()->add(new Sym(Sym::Type::Var, attrs, tok.location(), tok.text()));

    NodePtr value;
    const Type *type = nullptr;

    c.scanner.next(true);
    if(c.scanner.token().type() == Token::Type::Colon)
    {
        auto tn = TypeConstructs::type(c, true);
        type = c.types.insert(tn.get());
    }

    if(c.scanner.token().type() == Token::Type::Assign)
    {
        value = Expr::get(c, true);
        if(!type)
        {
            type = c.identifyType(value.get());
        }
    }

    if(!type)
    {
        throw Error(tok.location(), "no type - ", tok.text());
    }

    sym->setProperty("type", type);

    if(c.tree.current()->type() == Sym::Type::Class)
    {
        auto size = c.assertSize(tok.location(), type);
        c.tree.current()->setProperty("size", c.tree.current()->property("size").value<std::size_t>() + size);
    }

    block->nodes.push_back(new VarNode(tok.location(), sym, value));

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        var(c, block, attrs, true);
        return;
    }

    c.scanner.consume(Token::Type::Semicolon, false);
}

