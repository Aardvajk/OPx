#ifndef SCANNER_H
#define SCANNER_H

#include "scanner/Source.h"
#include "scanner/Token.h"

#include <pcx/scoped_ptr.h>

#include <vector>

class Scanner
{
public:
    Scanner();

    void push(Source *source);
    void pop();

    Token next(bool get);
    Token match(Token::Type type, bool get);
    Token consume(Token::Type type, bool get);

    Token token() const;

private:
    struct State
    {
        pcx::scoped_ptr<Source> src;
        Token tok;
    };

    std::vector<State> state;
};

#endif // SCANNER_H
