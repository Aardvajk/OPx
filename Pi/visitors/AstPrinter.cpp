#include "AstPrinter.h"

#include "scanner/Lexer.h"

#include "nodes/LiteralNode.h"
#include "nodes/StringNode.h"
#include "nodes/IdNode.h"
#include "nodes/AddrOfNode.h"

#include <sstream>

AstPrinter::AstPrinter(std::ostream &os) : os(os)
{
}

void AstPrinter::visit(LiteralNode<char> &node)
{
    os << "char(" << node.value << ")";
}

void AstPrinter::visit(LiteralNode<int> &node)
{
    os << "int(" << node.value << ")";
}

void AstPrinter::visit(LiteralNode<std::size_t> &node)
{
    os << "size(" << node.value << ")";
}

void AstPrinter::visit(StringNode &node)
{
    os << "\"" << Lexer::encodeString(node.value) << "\"";
}

void AstPrinter::visit(IdNode &node)
{
    os << node.value;
}

void AstPrinter::visit(AddrOfNode &node)
{
    os << "&";
    node.node->accept(*this);
}

std::string astReconstruct(Node *node)
{
    std::ostringstream os;

    AstPrinter ap(os);
    node->accept(ap);

    return os.str();
}
