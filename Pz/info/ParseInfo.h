#ifndef PARSEINFO_H
#define PARSEINFO_H

#include "syms/Sym.h"

#include <vector>

class ParseInfo
{
public:
    ParseInfo(){ }

    std::vector<Sym::Type> containers;
};

#endif // PARSEINFO_H
