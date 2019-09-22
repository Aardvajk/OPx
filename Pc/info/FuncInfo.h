#ifndef FUNCINFO_H
#define FUNCINFO_H

#include <cstdlib>

class FuncInfo
{
public:
    FuncInfo();

    std::size_t scopes;
    std::size_t labels;
};

#endif // FUNCINFO_H
