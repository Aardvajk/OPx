#ifndef PARSER_H
#define PARSER_H

#include "nodes/Node.h"

class Context;

namespace Parser
{

NodePtr build(Context &c);

}

#endif // PARSER_H
