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
        Id, IntLiteral, CharLiteral, StringLiteral,
        LeftBrace, RightBrace, LeftParen, RightParen, LeftSub, RightSub,
        Dot, Comma, Semicolon, Colon, Assign, Amp,
        RwClass, RwNamespace, RwUsing, RwPublic, RwPrivate, RwVar, RwArg, RwFunc, RwPtr,
        RwReturn,
        Eof, Invalid
    };

    Token();
    Token(Type type, Location location, Source::Char ch);
    Token(Type type, Location location, std::string text);

    Type type() const { return t; }
    std::string text() const { return s; }
    Location location() const { return n; }

    static const char *toString(Type type);

private:
    Type t;
    Location n;
    std::string s;
};

#endif // TOKEN_H
