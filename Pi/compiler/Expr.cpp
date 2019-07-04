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
        c.scanner.next(true);
        return new IdNode(tok.location(), tok.text());
    }
    else
    {
        static const std::vector<Token::Type> types =
        {
            Token::Type::RwChar,
            Token::Type::RwInt,
            Token::Type::RwSize,
            Token::Type::RwString
        };

        auto it = std::find(types.begin(), types.end(), tok.type());

        if(it != types.end())
        {
            auto i = *it;

            c.scanner.match(Token::Type::LeftParen, true);
            auto val = c.scanner.match(i == Token::Type::RwString ? Token::Type::StringLiteral : Token::Type::IntLiteral, true);
            c.scanner.consume(Token::Type::RightParen, true);

            switch(i)
            {
                case Token::Type::RwChar: return new LiteralNode<char>(val.location(), char(pcx::lexical_cast<int>(val.text())));
                case Token::Type::RwInt: return new LiteralNode<int>(val.location(), pcx::lexical_cast<int>(val.text()));
                case Token::Type::RwSize: return new LiteralNode<std::size_t>(val.location(), pcx::lexical_cast<std::size_t>(val.text()));
                case Token::Type::RwString: return new StringNode(val.location(), Lexer::decodeString(val.text()));

                default: break;
            }
        }

    }

    throw Error(tok.location(), "expression expected - ", tok.text());
}
