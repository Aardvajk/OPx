#ifndef FUNCPARSER_H
#define FUNCPARSER_H

class Context;
class BlockNode;

namespace FuncParser
{

void build(Context &c, BlockNode *block, bool get);

}

#endif // FUNCPARSER_H
