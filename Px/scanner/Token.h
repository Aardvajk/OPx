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
        Id, IntLiteral, CharLiteral, StringLiteral, SizeLiteral,
        LeftBrace, RightBrace, LeftParen, RightParen, LeftSub, RightSub,
        Dot, Comma, Semicolon, Colon, Assign, Amp, Star, Add, Sub, Div, Mod, Eq, Neq, Lt, LtEq, Gt, GtEq, Exclaim, And, Or, Ellipsis, Inc, Dec,
        AddEq, SubEq, MulEq, DivEq, ModEq,
        RwClass, RwNamespace, RwPublic, RwPrivate, RwVar, RwArg, RwFunc, RwPtr, RwRef, RwOperator, RwInclude, RwThis, RwPragma,
        RwWhile, RwIf, RwElse, RwFor,
        RwNew, RwDelete,
        RwTrue, RwFalse,
        RwReturn,
        RwChar, RwInt, RwSize, RwString,
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
