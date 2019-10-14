#include "Lexer.h"

#include "framework/Error.h"

#include "scanner/Source.h"
#include "scanner/Token.h"

#include <cctype>
#include <vector>

namespace
{

struct Reserved
{
    const char *text;
    Token::Type type;
};

static const std::vector<Reserved> pcReserved =
{
    { "class", Token::Type::RwClass },
    { "namespace", Token::Type::RwNamespace },
    { "var", Token::Type::RwVar },
    { "func", Token::Type::RwFunc },
    { "public", Token::Type::RwPublic },
    { "private", Token::Type::RwPrivate },
    { "free", Token::Type::RwFree },
    { "inline", Token::Type::RwInline },
    { "external", Token::Type::RwExternal },
    { "explicit", Token::Type::RwExplicit },
    { "ptr", Token::Type::RwPtr },
    { "ref", Token::Type::RwRef },
    { "operator", Token::Type::RwOperator },
    { "include", Token::Type::RwInclude },
    { "this", Token::Type::RwThis },
    { "pragma", Token::Type::RwPragma },
    { "const", Token::Type::RwConst },
    { "while", Token::Type::RwWhile },
    { "if", Token::Type::RwIf },
    { "else", Token::Type::RwElse },
    { "for", Token::Type::RwFor },
    { "break", Token::Type::RwBreak },
    { "new", Token::Type::RwNew },
    { "delete", Token::Type::RwDelete },
    { "true", Token::Type::RwTrue },
    { "false", Token::Type::RwFalse },
    { "return", Token::Type::RwReturn }
};

static const std::vector<Reserved> piReserved =
{
    { "var", Token::Type::RwVar },
    { "func", Token::Type::RwFunc },
    { "external", Token::Type::RwExternal },
    { "arg", Token::Type::RwArg },
    { "char", Token::Type::RwChar },
    { "int", Token::Type::RwInt },
    { "size", Token::Type::RwSize },
    { "string", Token::Type::RwString }
};

Source::Char translateEscapeChar(Source::Char ch)
{
    switch(ch)
    {
        case 'n': return '\n';
        case 't': return '\t';
        case '0': return '\0';
        case '\"': return '\"';
        case '\'': return '\'';

        default: return ch;
    }
}

Source::Char skip(Source &source)
{
    auto ch = source.get();
    while(std::isspace(ch))
    {
        ch = source.get();
    }

    if(ch == '/')
    {
        ch = source.get();
        if(ch == '/')
        {
            ch = source.get();
            while(ch && ch != '\n')
            {
                ch = source.get();
            }

            return skip(source);
        }
        else
        {
            source.unget(ch);
            return '/';
        }
    }

    return ch;
}

Token charConstant(Source &source, Location location)
{
    auto v = source.get();
    if(v == '\\')
    {
        v = translateEscapeChar(source.get());
    }

    auto ch = source.get();
    if(ch != '\'')
    {
        throw Error(location, "non-terminated character");
    }

    return Token(Token::Type::CharLiteral, location, v);
}

Token stringConstant(Source &source, Location location, std::string &s)
{
    auto ch = source.get();
    bool translated = false;

    if(ch == '\\')
    {
        ch = translateEscapeChar(source.get());
        translated = true;
    }

    while(ch != '\"' || translated)
    {
        if(!translated && (!ch || ch == '\n'))
        {
            throw Error(location, "non-terminated string constant");
        }

        s += char(ch);

        ch = source.get();

        translated = false;
        if(ch == '\\')
        {
            ch = translateEscapeChar(source.get());
            translated = true;
        }
    }

    ch = skip(source);
    if(ch == '\"')
    {
        return stringConstant(source, location, s);
    }

    source.unget(ch);
    return Token(Token::Type::StringLiteral, location, s);
}

Token::Type reserved(const std::vector<Reserved> &words, const std::string &text)
{
    for(auto &w: words)
    {
        if(w.text == text)
        {
            return w.type;
        }
    }

    return Token::Type::Id;
}

Token::Type reserved(Lexer::Mode mode, const std::string &text)
{
    switch(mode)
    {
        case Lexer::Mode::Pc: return reserved(pcReserved, text);
        case Lexer::Mode::Pi: return reserved(piReserved, text);

        default: return Token::Type::Id;
    }
}

Token dots(Source &source, Location location)
{
    auto ch = source.get();
    if(ch == '.')
    {
        ch = source.get();
        if(ch == '.')
        {
            return Token(Token::Type::Ellipsis, location, "...");
        }

        return Token(Token::Type::Invalid, location, "..");
    }

    source.unget(ch);
    return Token(Token::Type::Dot, location, '.');
}

typedef std::pair<Source::Char, Token::Type> char_type_pair;
Token speculate(Source &source, Location location, char_type_pair in, const std::vector<char_type_pair> &c)
{
    auto ch = source.get();
    for(auto &n: c)
    {
        if(n.first == ch)
        {
            return Token(n.second, location, pcx::str(char(in.first), char(ch)));
        }
    }

    source.unget(ch);
    return Token(in.second, location, char(in.first));
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
    if(ch == '[') return Token(Token::Type::LeftSub, loc, ch);
    if(ch == ']') return Token(Token::Type::RightSub, loc, ch);

    if(ch == ':') return Token(Token::Type::Colon, loc, ch);
    if(ch == ',') return Token(Token::Type::Comma, loc, ch);
    if(ch == ';') return Token(Token::Type::Semicolon, loc, ch);
    if(ch == '?') return Token(Token::Type::Question, loc, ch);

    if(ch == '+') return speculate(source, loc, { '+', Token::Type::Add }, { { '+', Token::Type::Inc }, { '=', Token::Type::AddEq } });
    if(ch == '-') return speculate(source, loc, { '-', Token::Type::Sub }, { { '-', Token::Type::Dec }, { '=', Token::Type::SubEq }, { '>', Token::Type::Arrow } });

    if(ch == '*') return speculate(source, loc, { '*', Token::Type::Star }, { { '=', Token::Type::MulEq } });
    if(ch == '/') return speculate(source, loc, { '/', Token::Type::Div }, { { '=', Token::Type::DivEq } });
    if(ch == '%') return speculate(source, loc, { '%', Token::Type::Mod }, { { '=', Token::Type::ModEq } });

    if(ch == '.') return dots(source, loc);

    if(ch == '=') return speculate(source, loc, { '=', Token::Type::Assign }, { { '=', Token::Type::Eq } });
    if(ch == '!') return speculate(source, loc, { '!', Token::Type::Exclaim }, { { '=', Token::Type::Neq } });

    if(ch == '<') return speculate(source, loc, { '<', Token::Type::Lt }, { { '=', Token::Type::LtEq }, { '<', Token::Type::LeftShift } });
    if(ch == '>') return speculate(source, loc, { '>', Token::Type::Gt }, { { '=', Token::Type::GtEq } });

    if(ch == '&') return speculate(source, loc, { '&', Token::Type::Amp }, { { '&', Token::Type::And } });
    if(ch == '|') return speculate(source, loc, { '|', Token::Type::Invalid }, { { '|', Token::Type::Or } });

    if(ch == '\'')
    {
        return charConstant(source, loc);
    }

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

        if(ch == 's')
        {
            return Token(Token::Type::SizeLiteral, loc, s);
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
            case '\"': s += "\\\""; break;
            case '\'': s += "\\'"; break;

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
        Source::Char ch = static_cast<Source::Char>(text[i]);

        if(ch == '\\' && i < text.length() - 1)
        {
            ch = translateEscapeChar(text[++i]);
        }

        s += static_cast<char>(ch);
    }

    return s;
}
