#include "Scanner.h"

#include "framework/Error.h"

#include "scanner/Lexer.h"

#include <algorithm>

Scanner::Scanner(Lexer::Mode mode) : mode(mode)
{
}

void Scanner::push(Source *source)
{
    state.push_back({ source, Token() });
}

void Scanner::pop()
{
    state.pop_back();
}

Token Scanner::next(bool get)
{
    if(get)
    {
        state.back().tok = Lexer::next(mode, *state.back().src);
        if(state.back().tok.type() == Token::Type::Invalid)
        {
            throw Error(state.back().tok.location(), "invalid token - ", state.back().tok.text());
        }
    }

    return state.back().tok;
}

Token Scanner::match(Token::Type type, bool get)
{
    auto t = next(get);
    if(t.type() != type)
    {
        throw Error(t.location(), Token::toString(type), " expected - ", t.text());
    }

    return t;
}

Token Scanner::consume(Token::Type type, bool get)
{
    match(type, get);
    return next(true);
}

Token Scanner::token() const
{
    return state.back().tok;
}

Location::Id Scanner::sourceId() const
{
    return state.back().src->location().id();
}
