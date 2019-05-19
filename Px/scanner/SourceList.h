#ifndef SOURCELIST_H
#define SOURCELIST_H

#include "scanner/Location.h"

#include <vector>
#include <string>

class SourceList
{
public:
    SourceList();

    Location::Id id(const std::string &path);

    std::string path(Location::Id id) const;

private:
    std::vector<std::string> v;
};

#endif // SOURCELIST_H
