#ifndef COMMONGENERATOR_H
#define COMMONGENERATOR_H

#include "scanner/Location.h"

#include <iostream>

class Context;
class Node;
class Type;
class Sym;

namespace CommonGenerator
{

void generateBooleanExpression(Context &c, std::ostream &os, Node *node);
void generateParameter(Context &c, std::ostream &os, Node *node, Type *type);
void generateAllDefaultParameters(Context &c, std::ostream &os, Sym *sym);

void processTempDestructs(Context &c, std::ostream &os, Location location);

}

#endif // COMMONGENERATOR_H
