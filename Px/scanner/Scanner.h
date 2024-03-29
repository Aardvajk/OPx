#ifndef SCANNER_H
#define SCANNER_H

#include "scanner/Source.h"
#include "scanner/Token.h"
#include "scanner/Lexer.h"

#include <pcx/scoped_ptr.h>

#include <vector>

class Scanner
{
public:
    explicit Scanner(Lexer::Mode mode);

    void push(Source *source);
    void pop();

    Token next(bool get);
    Token match(Token::Type type, bool get);
    Token consume(Token::Type type, bool get);

    Token token() const;
    Location::Id sourceId() const;

private:
    struct State
    {
        pcx::scoped_ptr<Source> src;
        Token tok;
    };

    Lexer::Mode mode;
    std::vector<State> state;
};

#endif // SCANNER_H
