#ifndef TESTCONSTRUCTS_H
#define TESTCONSTRUCTS_H

class Context;
class BlockNode;

namespace TestConstructs
{

void lookup(Context &c, bool get);
void triggerError(Context &c, BlockNode *block, bool get);

}

#endif // TESTCONSTRUCTS_H
