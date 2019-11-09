#ifndef DECLARATIONPARSER_H
#define DECLARATIONPARSER_H

class Context;
class BlockNode;

namespace DeclarationParser
{

void buildNamespace(Context &c, BlockNode *block, bool get);
void buildClass(Context &c, BlockNode *block, bool get);
void buildVar(Context &c, BlockNode *block, bool get);
void buildTerminatedVar(Context &c, BlockNode *block, bool get);
void buildFunction(Context &c, BlockNode *block, bool get);

}

#endif // DECLARATIONPARSER_H
