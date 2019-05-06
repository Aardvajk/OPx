#ifndef LEXER_H
#define LEXER_H

class Token;
class Source;

namespace Lexer
{

Token next(Source &source);

}

#endif // LEXER_H
