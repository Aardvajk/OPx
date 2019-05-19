#include "Lexer.h"

#include "error/Error.h"

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

Token stringConstant(Source &source, Location location, std::string &s)
{
    auto ch = source.get();
    while(ch != '\"')
    {
        if(!ch || ch == '\n')
        {
            throw Error(location, "non-terminated string constant");
        }

        s += char(ch);
        ch = source.get();
    }

    ch = skip(source);
    if(ch == '\"')
    {
        return stringConstant(source, location, s);
    }

    source.unget(ch);
    return Token(Token::Type::StringLiteral, location, s);
}

Token::Type reserved(Lexer::Mode mode, const std::string &text)
{
    if(mode == Lexer::Mode::Pc)
    {
        static const char *s[] = { "class", "namespace", "using", "public", "private", "var", "func", "ptr",
                                   "lookup", "trigger_error",
                                   "" };

        for(int i = 0; s[i][0]; ++i)
        {
            if(text == s[i])
            {
                return static_cast<Token::Type>(static_cast<int>(Token::Type::RwClass) + i);
            }
        }
    }
    else if(mode == Lexer::Mode::Pi)
    {
        if(text == "var") return Token::Type::RwVar;
        if(text == "func") return Token::Type::RwFunc;
    }

    return Token::Type::Id;
}

}

Token Lexer::next(Mode mode, Source &source)
{
    auto ch = skip(source);
    auto loc = source.location();

    if(!ch) return Token(Token::Type::Eof, loc, "EOF");

    if(ch == '{') return Token(Token::Type::LeftBrace, loc, ch);
    if(ch == '}') return Token(Token::Type::RightBrace, loc, ch);
    if(ch == '(') return Token(Token::Type::LeftParen, loc, ch);
    if(ch == ')') return Token(Token::Type::RightParen, loc, ch);

    if(ch == '.') return Token(Token::Type::Dot, loc, ch);
    if(ch == ':') return Token(Token::Type::Colon, loc, ch);
    if(ch == ',') return Token(Token::Type::Comma, loc, ch);
    if(ch == ';') return Token(Token::Type::Semicolon, loc, ch);
    if(ch == '=') return Token(Token::Type::Assign, loc, ch);

    if(ch == '\"')
    {
        std::string s;
        return stringConstant(source, loc, s);
    }

    if(std::isalpha(ch) || ch == '_')
    {
        std::string s;
        while(std::isalnum(ch) || ch == '_')
        {
            s += char(ch);
            ch = source.get();
        }

        source.unget(ch);
        return Token(reserved(mode, s), loc, s);
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

std::string Lexer::encodeString(const std::string &text)
{
    std::string s;

    for(std::size_t i = 0; i < text.length(); ++i)
    {
        auto ch = text[i];

        switch(ch)
        {
            case '\n': s += "\\n"; break;
            case '\t': s += "\\t"; break;
            case '\0': s += "\\0"; break;

            default: s += ch;
        }
    }

    return s;
}

std::string Lexer::decodeString(const std::string &text)
{
    std::string s;

    for(std::size_t i = 0; i < text.length(); ++i)
    {
        auto ch = text[i];

        if(ch == '\\' && i < text.length() - 1)
        {
            ch = text[++i];
            switch(ch)
            {
                case 'n': ch = '\n'; break;
                case 't': ch = '\t'; break;
                case '0': ch = '\0'; break;
            }
        }

        s += ch;
    }

    return s;
}
