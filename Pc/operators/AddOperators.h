#ifndef ADDOPERATORS_H
#define ADDOPERATORS_H

#include <iostream>

class Context;
class BinaryNode;

namespace AddOperators
{

std::size_t generate(Context &c, std::ostream &os, BinaryNode &node);

}

#endif // ADDOPERATORS_H
