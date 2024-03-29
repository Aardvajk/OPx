#ifndef FUNCINFO_H
#define FUNCINFO_H

#include "info/Temp.h"
#include "info/TempDestruct.h"

#include <cstdlib>
#include <string>
#include <vector>
#include <utility>

class Node;
class Type;

class FuncInfo
{
public:
    FuncInfo();

    std::string nextLabel();
    std::string nextLabelQuoted();

    std::size_t scopes;
    std::size_t labels;

    std::vector<std::vector<Node*> > destructs;
    std::vector<Temp> temps;
    std::vector<TempDestruct> tempDestructs;
};

#endif // FUNCINFO_H
