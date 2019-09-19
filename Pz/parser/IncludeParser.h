#ifndef INCLUDEPARSER_H
#define INCLUDEPARSER_H

class Context;
class BlockNode;

namespace IncludeParser
{

void build(Context &c, BlockNode *block, bool get);

}

#endif // INCLUDEPARSER_H
