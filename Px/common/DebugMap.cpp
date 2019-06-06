#include "DebugMap.h"

#include "framework/Error.h"

#include "scanner/Lexer.h"
#include "scanner/Scanner.h"

#include <pcx/lexical_cast.h>

DebugMap::DebugMap()
{
}

void DebugMap::begin(char type, const std::string &name, std::size_t size, pcx::optional<Callback> callback)
{
    v.push_back({ type, name, size, { } });
    cb = callback;
}

void DebugMap::read(std::istream &is)
{
    Scanner sc(Lexer::Mode::Map);
    sc.push(new Source(0, is));

    sc.next(true);
    while(sc.token().type() != Token::Type::Eof)
    {
        auto id = sc.match(Token::Type::Id, false);

        auto type = id.text()[0];
        if(type != 'V' && type != 'F')
        {
            throw Error("type expected - ", id.text());
        }

        sc.consume(Token::Type::Comma, true);
        auto name = sc.match(Token::Type::StringLiteral, false);

        sc.consume(Token::Type::Comma, true);
        auto size = sc.match(Token::Type::IntLiteral, false);

        v.push_back({ type, name.text(), pcx::lexical_cast<std::size_t>(size.text()), { } });

        sc.consume(Token::Type::LeftBrace, true);
        while(sc.token().type() != Token::Type::RightBrace)
        {
            auto addr = sc.match(Token::Type::IntLiteral, false);

            sc.consume(Token::Type::Comma, true);
            auto text = sc.match(Token::Type::StringLiteral, false);

            v.back().comments.push_back({ pcx::lexical_cast<std::size_t>(addr.text()), Lexer::decodeString(text.text()) });

            sc.consume(Token::Type::Semicolon, true);
        }

        sc.consume(Token::Type::RightBrace, false);
    }
}

void DebugMap::write(std::ostream &os) const
{
    for(auto &e: v)
    {
        os << e.type << ", \"" << Lexer::encodeString(e.name) << "\", " << e.size << "\n";
        os << "{\n";

        for(auto &c: e.comments)
        {
            os << "    " << c.address << ", \"" << Lexer::encodeString(c.text) << "\";\n";
        }

        os << "}\n";
    }
}
