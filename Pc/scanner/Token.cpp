#include "Token.h"

const char *Token::toString(Type type)
{
    switch(type)
    {
        case Type::Id: return "identifier";

        case Type::LeftBrace: return "left brace";
        case Type::RightBrace: return "right brace";

        case Type::Dot: return "dot";
        case Type::Semicolon: return "semicolon";

        default: return "(unknown)";
    }
}

Token::Type Token::reserved(const std::string &text)
{
    static const char *s[] = { "class", "using", "" };

    for(int i = 0; s[i][0]; ++i)
    {
        if(text == s[i])
        {
            return static_cast<Token::Type>(static_cast<int>(Token::Type::RwClass) + i);
        }
    }

    return Token::Type::Id;
}
