#include "SourceList.h"

#include <algorithm>

SourceList::SourceList()
{
    v.push_back("(invalid)");
}

Location::Id SourceList::id(const std::string &path)
{
    auto i = std::find(v.begin(), v.end(), path);
    if(i == v.end())
    {
        v.push_back(path);
        return Location::Id(v.size() - 1);
    }

    return Location::Id(i - v.begin());
}

std::string SourceList::path(Location::Id id) const
{
    return v[id];
}

bool SourceList::contains(const std::string &path) const
{
    return std::find(v.begin(), v.end(), path) != v.end();
}
