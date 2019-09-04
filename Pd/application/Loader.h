#ifndef LOADER_H
#define LOADER_H

#include "common/Object.h"

namespace Loader
{

Object::Unit loadObjectUnit(const std::string &path);

}

#endif // LOADER_H
