#ifndef COMMONGENERATOR_H
#define COMMONGENERATOR_H

#include <iostream>

class Context;
class Node;
class Type;

namespace CommonGenerator
{

void generateBooleanExpression(Context &c, std::ostream &os, Node &node);
void generateParameter(Context &c, std::ostream &os, Node &node, Type *type);

}

#endif // COMMONGENERATOR_H
