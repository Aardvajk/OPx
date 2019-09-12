#include "Parser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"

#include "parser/CommonParser.h"

namespace
{

void construct(Context &c, BlockNode *block, bool get)
{
    auto n = CommonParser::extendedName(c, get);
    block->push_back(n);

    c.scanner.consume(Token::Type::Semicolon, false);

//    auto tok = c.scanner.next(get);
//    switch(tok.type())
//    {
//        default: throw Error(tok.location(), "construct expected - ", tok.text());
//    }
}

}

NodePtr Parser::build(Context &c)
{
    auto block = new BlockNode(Location());
    NodePtr n(block);

    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::Eof)
    {
        construct(c, block, false);
    }

    return n;
}
