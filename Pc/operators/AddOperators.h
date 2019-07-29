#ifndef ADDOPERATORS_H
#define ADDOPERATORS_H

#include <iostream>

class Context;
class BinaryNode;

namespace AddOperators
{

void generate(Context &c, std::ostream &os, BinaryNode &node);

}

#endif // ADDOPERATORS_H
