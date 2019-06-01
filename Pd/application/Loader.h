#ifndef LOADER_H
#define LOADER_H

#include "common/Object.h"

#include "framework/Segment.h"

namespace Loader
{

Object::Unit loadObjectUnit(const std::string &path, std::vector<Segment> &segments);

}

#endif // LOADER_H
