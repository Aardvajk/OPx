#ifndef PRAGMAPARSER_H
#define PRAGMAPARSER_H

class Context;
class BlockNode;

namespace PragmaParser
{

void build(Context &c, BlockNode *block, bool get);

}

#endif // PRAGMAPARSER_H
