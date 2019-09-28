#ifndef MATHOPERATORS_H
#define MATHOPERATORS_H

#include <iostream>

class Context;
class BinaryNode;
class UnaryNode;

namespace MathOperators
{

std::size_t generateAdd(Context &c, std::ostream &os, BinaryNode &node);
std::size_t generateSub(Context &c, std::ostream &os, BinaryNode &node);
std::size_t generateMulDivMod(Context &c, std::ostream &os, BinaryNode &node);
std::size_t generateNotNeg(Context &c, std::ostream &os, UnaryNode &node);

}

#endif // MATHOPERATORS_H
