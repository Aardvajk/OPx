#ifndef COMMONDECORATOR_H
#define COMMONDECORATOR_H

class Context;
class FuncNode;
class Sym;
class Node;
class Type;

namespace CommonDecorator
{

Sym *decorateFuncSignature(Context &c, FuncNode &node);
Sym *searchCallableByType(Context &c, Node &node, const Type *expectedType);

}

#endif // COMMONDECORATOR_H
