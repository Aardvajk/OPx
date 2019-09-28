#ifndef FUNCINFO_H
#define FUNCINFO_H

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
    std::vector<std::pair<std::string, Type*> > temps;

    std::vector<std::pair<std::string, Type*> > tempDestructs;
};

#endif // FUNCINFO_H
