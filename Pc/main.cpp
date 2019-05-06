#include "application/Error.h"

#include "scanner/Source.h"
#include "scanner/Token.h"
#include "scanner/Lexer.h"

#include <iostream>
#include <fstream>

int main()
{
    try
    {
        Source src(0, new std::ifstream("C:/Projects/Px/Px/Pc/script.txt"));

        auto tok = Lexer::next(src);
        while(tok.type() != Token::Type::Eof)
        {
            std::cout << tok.location().column() << ", " << tok.location().line() << ": [" << tok.text() << "]\n";
            tok = Lexer::next(src);
        }
    }

    catch(const Error &error)
    {
        std::cout << "error " << error.location().column() << ", " << error.location().line() << ": " << error.what() << "\n";
    }
}
