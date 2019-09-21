#ifndef COMPAREOPERATORS_H
#define COMPAREOPERATORS_H

#include <iostream>

class Context;
class BinaryNode;

namespace CompareOperators
{

std::size_t generate(Context &c, std::ostream &os, BinaryNode &node);

}

#endif // COMPAREOPERATORS_H
