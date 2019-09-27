#ifndef OPERATORCALLDECORATE_H
#define OPERATORCALLDECORATE_H

#include "nodes/Node.h"

class Context;

namespace OperatorCallDecorate
{

NodePtr generate(Context &c, Node &node, NodePtr first, NodeList &params, const std::string &op);

}

#endif // OPERATORCALLDECORATE_H
