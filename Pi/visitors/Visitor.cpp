#include "Visitor.h"

Visitor::~Visitor()
{
}

void Visitor::visit(LiteralNode<char> &node)
{
}

void Visitor::visit(LiteralNode<int> &node)
{
}

void Visitor::visit(LiteralNode<std::size_t> &node)
{
}

void Visitor::visit(StringNode &node)
{
}

void Visitor::visit(IdNode &node)
{
}

void Visitor::visit(AddrOfNode &node)
{
}
