#include "Token.h"

Token::Token() : t(Type::Invalid)
{
}

Token::Token(Type type, Location location, Source::Char ch) : t(type), n(location), s(std::string(1, char(ch)))
{
}

Token::Token(Type type, Location location, std::string text) : t(type), n(location), s(std::move(text))
{
}

const char *Token::toString(Type type)
{
    switch(type)
    {
        case Type::Id: return "identifier";
        case Type::StringLiteral: return "string literal";

        case Type::LeftBrace: return "left brace";
        case Type::RightBrace: return "right brace";
        case Type::LeftParen: return "left bracket";
        case Type::RightParen: return "right bracket";

        case Type::Dot: return "dot";
        case Type::Comma: return "comma";
        case Type::Semicolon: return "semicolon";
        case Type::Colon: return "colon";

        default: return "(unknown)";
    }
}

Token::Type Token::reserved(const std::string &text)
{
    static const char *s[] = { "class", "namespace", "using", "public", "private", "func", "ptr",
                               "lookup", "trigger_error",
                               "" };

    for(int i = 0; s[i][0]; ++i)
    {
        if(text == s[i])
        {
            return static_cast<Token::Type>(static_cast<int>(Token::Type::RwClass) + i);
        }
    }

    return Token::Type::Id;
}
