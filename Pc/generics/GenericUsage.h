#ifndef GENERICUSAGE_H
#define GENERICUSAGE_H

#include <vector>

class FuncNode;
class Type;

class GenericUsage
{
public:
    GenericUsage(FuncNode *node, std::vector<Type*> types) : node(node), types(std::move(types)) { }

    FuncNode *node;
    std::vector<Type*> types;
};

#endif // GENERICUSAGE_H
