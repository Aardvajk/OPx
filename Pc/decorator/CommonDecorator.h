#ifndef COMMONDECORATOR_H
#define COMMONDECORATOR_H

#include <vector>

class Context;
class Node;
class Type;
class Sym;

namespace CommonDecorator
{

std::vector<Sym*> searchCallable(Context &c, Node *node, Type *expectedType);

}

#endif // COMMONDECORATOR_H
