#ifndef COMMONGENERATOR_H
#define COMMONGENERATOR_H

#include <iostream>

class Context;
class Node;

namespace CommonGenerator
{

void generateBooleanExpression(Context &c, std::ostream &os, Node &node);

}

#endif // COMMONGENERATOR_H
