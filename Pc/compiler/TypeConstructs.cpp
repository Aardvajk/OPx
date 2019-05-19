#include "TypeConstructs.h"

#include "error/Error.h"

#include "application/Context.h"

#include "compiler/CommonConstructs.h"

namespace
{

void args(Context &c, pcx::ptr_vector<Type> &v, bool get)
{
    v.push_back(TypeConstructs::type(c, get).release());

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        args(c, v, true);
    }
}

TypePtr primary(Context &c, bool get)
{
    auto tok = c.scanner.next(get);

    if(tok.type() == Token::Type::RwFunc)
    {
        TypePtr tn(new Type());

        c.scanner.consume(Token::Type::LeftParen, true);
        if(c.scanner.token().type() != Token::Type::RightParen)
        {
            args(c, tn->args, false);
        }

        c.scanner.consume(Token::Type::RightParen, false);

        c.scanner.match(Token::Type::Colon, false);
        tn->returnType = TypeConstructs::type(c, true).release();

        return tn;
    }
    else
    {
        auto nn = CommonConstructs::name(c, false);

        auto sym = c.find(SymFinder::Policy::Full, nn.get());
        if(!Sym::isType(sym->type()))
        {
            throw Error(nn->location(), "type expected - ", sym->fullname());
        }

        return new Type(sym);
    }
}

}

TypePtr TypeConstructs::type(Context &c, bool get)
{
    unsigned ptr = 0;

    auto tok = c.scanner.next(get);
    while(tok.type() == Token::Type::RwPtr)
    {
        ++ptr;
        tok = c.scanner.next(true);
    }

    auto tn = primary(c, false);

    tn->ptr = ptr;

    return tn;
}
