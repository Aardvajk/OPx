#ifndef FUNCINFO_H
#define FUNCINFO_H

#include <cstdlib>
#include <vector>

class Node;

class FuncInfo
{
public:
    FuncInfo();

    std::size_t scopes;
    std::size_t labels;

    std::vector<std::vector<Node*> > destructs;
};

#endif // FUNCINFO_H
