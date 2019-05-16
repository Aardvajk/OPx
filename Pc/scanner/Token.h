#ifndef TOKEN_H
#define TOKEN_H

#include "scanner/Location.h"
#include "scanner/Source.h"

#include <string>

class Token
{
public:
    enum class Type
    {
        Id, IntLiteral, StringLiteral,
        LeftBrace, RightBrace, LeftParen, RightParen,
        Dot, Comma, Semicolon, Colon, Assign,
        RwClass, RwNamespace, RwUsing, RwPublic, RwPrivate, RwFunc, RwPtr,
        RwLookup, RwTriggerError,
        Eof, Invalid
    };

    Token();
    Token(Type type, Location location, Source::Char ch);
    Token(Type type, Location location, std::string text);

    Type type() const { return t; }
    std::string text() const { return s; }
    Location location() const { return n; }

    static const char *toString(Type type);
    static Type reserved(const std::string &text);

private:
    Type t;
    Location n;
    std::string s;
};

#endif // TOKEN_H
