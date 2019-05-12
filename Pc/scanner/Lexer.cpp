#include "Lexer.h"

#include "application/Error.h"

#include "scanner/Source.h"
#include "scanner/Token.h"

#include <cctype>

namespace
{

Source::Char skip(Source &source)
{
    auto ch = source.get();
    while(std::isspace(ch))
    {
        ch = source.get();
    }

    return ch;
}

}

Token Lexer::next(Source &source)
{
    auto ch = skip(source);
    auto loc = source.location();

    if(!ch) return Token(Token::Type::Eof, loc, "EOF");

    if(ch == '{') return Token(Token::Type::LeftBrace, loc, ch);
    if(ch == '}') return Token(Token::Type::RightBrace, loc, ch);

    if(ch == '.') return Token(Token::Type::Dot, loc, ch);
    if(ch == ';') return Token(Token::Type::Semicolon, loc, ch);
    if(ch == '=') return Token(Token::Type::Assign, loc, ch);

    if(std::isalpha(ch))
    {
        std::string s;
        while(std::isalnum(ch))
        {
            s += char(ch);
            ch = source.get();
        }

        source.unget(ch);
        return Token(Token::reserved(s), loc, s);
    }

    if(std::isdigit(ch))
    {
        std::string s;
        while(std::isdigit(ch))
        {
            s += char(ch);
            ch = source.get();
        }

        source.unget(ch);
        return Token(Token::Type::IntLiteral, loc, s);
    }

    throw Error(loc, "invalid character - ", char(ch));
}
