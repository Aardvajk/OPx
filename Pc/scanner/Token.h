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
        Id, IntLiteral,
        LeftBrace, RightBrace,
        Dot, Semicolon, Assign,
        RwClass, RwUsing,
        Eof, Invalid
    };

    Token() : t(Type::Invalid) { }
    Token(Type type, Location location, Source::Char ch) : t(type), n(location), s(std::string(1, char(ch))) { }
    Token(Type type, Location location, std::string text) : t(type), n(location), s(std::move(text)) { }

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
