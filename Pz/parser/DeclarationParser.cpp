#include "DeclarationParser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/ClassNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ScopeNode.h"

#include "parser/CommonParser.h"
#include "parser/TypeParser.h"

void DeclarationParser::buildNamespace(Context &c, BlockNode *block, bool get)
{
    auto name = CommonParser::name(c, get);

    auto n = new NamespaceNode(name->location(), name);
    block->push_back(n);

    n->body = CommonParser::blockContents(c, n->location(), false);
}

void DeclarationParser::buildClass(Context &c, BlockNode *block, bool get)
{
    auto name = CommonParser::name(c, get);

    auto n = new ClassNode(name->location(), name);
    block->push_back(n);

    if(c.scanner.token().type() == Token::Type::LeftBrace)
    {
        n->body = CommonParser::blockContents(c, n->location(), false);
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }
}

void DeclarationParser::buildFunction(Context &c, BlockNode *block, bool get)
{
    auto name = CommonParser::name(c, get);

    auto n = new FuncNode(name->location(), name);
    block->push_back(n);

    c.scanner.match(Token::Type::LeftParen, false);
    c.scanner.match(Token::Type::RightParen, true);

    auto tok = c.scanner.next(true);
    if(tok.type() == Token::Type::Colon)
    {
        n->type = TypeParser::build(c, true);
    }

    if(c.scanner.token().type() == Token::Type::LeftBrace)
    {
        auto scope = new ScopeNode(n->location());
        n->body = scope;

        scope->body = CommonParser::blockContents(c, n->location(), false);
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }
}
