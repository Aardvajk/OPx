#ifndef LOADER_H
#define LOADER_H

#include "common/Object.h"

namespace Loader
{

Object::Unit loadObjectUnit(const std::string &path, std::vector<std::vector<char> > &segments);

}

#endif // LOADER_H
