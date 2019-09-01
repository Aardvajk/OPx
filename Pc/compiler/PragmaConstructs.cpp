#include "PragmaConstructs.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/PragmaNode.h"

void PragmaConstructs::entity(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.match(Token::Type::RwPragma, get);

    std::string cmd;
    std::string arg;

    cmd = c.scanner.match(Token::Type::Id, true).text();

    c.scanner.next(true);
    if(c.scanner.token().type() != Token::Type::Semicolon)
    {
        arg = c.scanner.match(Token::Type::StringLiteral, false).text();
        c.scanner.next(true);
    }

    block->push_back(new PragmaNode(tok.location(), cmd, arg));

    c.scanner.consume(Token::Type::Semicolon, false);
}

void PragmaConstructs::execute(Context &c, PragmaNode &node)
{
    if(node.cmd == "push")
    {
        c.args.push_back(c.args.back());
    }
    else if(node.cmd == "pop")
    {
        if(c.args.size() < 2)
        {
            throw Error(node.location(), "pragma pop without push");
        }

        c.args.pop_back();
    }
    else if(node.cmd == "set")
    {
        c.args.back().process(node.arg);
    }
    else
    {
        throw Error(node.location(), "unknown pragma - ", node.cmd);
    }
}

