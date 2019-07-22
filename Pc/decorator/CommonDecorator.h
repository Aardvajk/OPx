#ifndef COMMONDECORATOR_H
#define COMMONDECORATOR_H

class Context;
class FuncNode;
class Sym;

namespace CommonDecorator
{

Sym *decorateFuncSignature(Context &c, FuncNode &node);

}

#endif // COMMONDECORATOR_H
