#include "Scanner.h"

#include "framework/Error.h"

#include "scanner/Lexer.h"

#include <algorithm>

Scanner::Scanner(Lexer::Mode mode) : mode(mode), braces(0)
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

        switch(state.back().tok.type())
        {
            case Token::Type::LeftBrace: ++braces; break;
            case Token::Type::RightBrace: --braces; break;

            default: break;
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

void Scanner::recover(bool get)
{
    next(get);
    while(braces)
    {
        next(true);
    }

    next(true);
}

Token Scanner::token() const
{
    return state.back().tok;
}
