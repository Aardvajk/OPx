#ifndef PRAGMACONSTRUCTS_H
#define PRAGMACONSTRUCTS_H

class Context;
class BlockNode;
class PragmaNode;

namespace PragmaConstructs
{

void entity(Context &c, BlockNode *block, bool get);
void execute(Context &c, PragmaNode &node);

}

#endif // PRAGMACONSTRUCTS_H
