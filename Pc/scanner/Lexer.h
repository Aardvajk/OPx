#ifndef LEXER_H
#define LEXER_H

#include <string>

class Token;
class Source;

namespace Lexer
{

Token next(Source &source);

std::string encodeString(const std::string &text);
std::string decodeString(const std::string &text);

}

#endif // LEXER_H
