#include "Scanner.h"

#include "application/Error.h"

#include "scanner/Lexer.h"

#include <algorithm>

Scanner::Scanner()
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
        state.back().tok = Lexer::next(*state.back().src);
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
