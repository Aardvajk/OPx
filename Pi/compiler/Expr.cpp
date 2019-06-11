#include "Expr.h"

#include "framework/Error.h"

#include "scanner/Lexer.h"

#include "application/Context.h"

#include "nodes/LiteralNode.h"
#include "nodes/StringNode.h"
#include "nodes/IdNode.h"
#include "nodes/AddrOfNode.h"

#include <pcx/lexical_cast.h>

#include <algorithm>

NodePtr Expr::get(Context &c, bool get)
{
    auto tok = c.scanner.next(get);

    if(tok.type() == Token::Type::Amp)
    {
        auto n = Expr::get(c, true);
        return new AddrOfNode(tok.location(), n);
    }
    else if(tok.type() == Token::Type::Id || tok.type() == Token::Type::StringLiteral)
    {
        static const std::vector<std::string> types =
        {
            "char",
            "string",
            "int",
            "size"
        };

        auto it = std::find(types.begin(), types.end(), tok.text());

        if(it != types.end())
        {
            auto i = it - types.begin();

            c.scanner.match(Token::Type::LeftParen, true);
            auto val = c.scanner.match(i == 1 ? Token::Type::StringLiteral : Token::Type::IntLiteral, true);
            c.scanner.consume(Token::Type::RightParen, true);

            switch(i)
            {
                case 0: return new LiteralNode<char>(val.location(), char(pcx::lexical_cast<int>(val.text())));
                case 1: return new StringNode(val.location(), Lexer::decodeString(val.text()));
                case 2: return new LiteralNode<int>(val.location(), pcx::lexical_cast<int>(val.text()));
                case 3: return new LiteralNode<std::size_t>(val.location(), pcx::lexical_cast<std::size_t>(val.text()));
            }
        }

        c.scanner.next(true);
        return new IdNode(tok.location(), tok.text());
    }

    throw Error(tok.location(), "expression expected - ", tok.text());
}
