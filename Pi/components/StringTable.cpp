#include "StringTable.h"

StringTable::StringTable()
{
}

std::size_t StringTable::insert(const std::string &s)
{
    for(std::size_t i = 0; i < v.size(); ++i)
    {
        if(v[i] == s)
        {
            return i;
        }
    }

    v.push_back(s);
    return v.size() - 1;
}
