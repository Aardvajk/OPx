#include "TypeConstructs.h"

#include "application/Context.h"
#include "application/Error.h"

#include "compiler/CommonConstructs.h"

namespace
{

TypePtr primary(Context &c, bool get)
{
    auto tok = c.scanner.next(get);

    if(tok.type() == Token::Type::RwFunc)
    {
        TypePtr tn(new Type());

        c.scanner.match(Token::Type::LeftParen, true);

        do
        {
            tn->args.push_back(TypeConstructs::type(c, true).release());
        }
        while(c.scanner.token().type() == Token::Type::Comma);

        c.scanner.consume(Token::Type::RightParen, false);

        c.scanner.match(Token::Type::Colon, false);
        tn->returnType = TypeConstructs::type(c, true).release();

        return tn;
    }
    else
    {
        auto nn = CommonConstructs::name(c, false);

        auto sym = c.find(SymFinder::Policy::Full, nn.get());
        if(sym->type() != Sym::Type::Class)
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
