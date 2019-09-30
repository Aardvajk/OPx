#include "IncludeParser.h"

#include "application/Context.h"

#include "parser/Parser.h"

#include <pcx/str.h>
#include <pcx/split_str.h>
#include <pcx/filesys.h>

namespace
{

std::string search(const std::vector<std::string> &search, const std::string &path)
{
    for(auto s: search)
    {
        auto p = pcx::str(s, '/', path);
        if(pcx::filesys::exists(p))
        {
            return p;
        }
    }

    return { };
}

void process(Context &c, BlockNode *block, bool force, bool get)
{
    auto tok = c.scanner.match(Token::Type::StringLiteral, get);
    auto path = tok.text();

    if(!pcx::filesys::exists(path))
    {
        path = search(c.values("I"), path);
        if(path.empty())
        {
            throw Error(tok.location(), "unable to locate - \"", tok.text(), "\"");
        }
    }

    if(force || !c.sources.contains(path))
    {
        c.open(path);

        c.scanner.next(true);
        while(c.scanner.token().type() != Token::Type::Eof)
        {
            Parser::construct(c, block, false);
        }

        c.scanner.pop();
    }

    c.scanner.next(true);
    if(c.scanner.token().type() == Token::Type::Comma)
    {
        process(c, block, force, true);
    }
}

}

void IncludeParser::build(Context &c, BlockNode *block, bool get)
{
    if(c.containers.back() != Sym::Type::Namespace)
    {
        throw Error(c.scanner.token().location(), "invalid include");
    }

    bool force = false;

    auto tok = c.scanner.next(get);
    if(tok.type() == Token::Type::RwInline)
    {
        force = true;
        c.scanner.next(true);
    }

    c.scanner.consume(Token::Type::LeftParen, false);

    if(c.scanner.token().type() != Token::Type::RightParen)
    {
        process(c, block, force, false);
    }

    c.scanner.match(Token::Type::RightParen, false);
    c.scanner.consume(Token::Type::Semicolon, true);
}
