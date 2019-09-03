#include "IncludeConstructs.h"

#include "application/Context.h"

#include "compiler/Compiler.h"

#include <pcx/str.h>
#include <pcx/split_str.h>
#include <pcx/filesys.h>

namespace
{

bool searchAndOpen(Context &c, const std::vector<std::string> &search, const std::string &path)
{
    for(auto s: search)
    {
        auto p = pcx::str(s, '/', path);
        if(pcx::filesys::exists(p))
        {
            c.open(p);
            return true;
        }
    }

    return false;
}

void process(Context &c, BlockNode *block, bool get)
{
    auto path = c.scanner.match(Token::Type::StringLiteral, get);

    if(pcx::filesys::exists(path.text()))
    {
        c.open(path.text());
    }
    else
    {
        if(!searchAndOpen(c, c.args.back()["I"], path.text()))
        {
            throw Error(path.location(), "unable to locate - \"", path.text(), "\"");
        }
    }

    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::Eof)
    {
        Compiler::construct(c, block, false);
    }

    c.scanner.pop();

    c.scanner.next(true);
    if(c.scanner.token().type() == Token::Type::Comma)
    {
        process(c, block, true);
    }
}

}

void IncludeConstructs::entity(Context &c, BlockNode *block, bool get)
{
    c.scanner.match(Token::Type::RwInclude, get);
    c.scanner.consume(Token::Type::LeftParen, true);

    if(c.scanner.token().type() != Token::Type::RightParen)
    {
        process(c, block, false);
    }

    c.scanner.match(Token::Type::RightParen, false);
    c.scanner.consume(Token::Type::Semicolon, true);
}
