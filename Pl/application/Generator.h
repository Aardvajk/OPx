#ifndef GENERATOR_H
#define GENERATOR_H

class Context;

#include <string>
#include <vector>

namespace Generator
{

void generate(Context &c, const std::vector<std::string> &paths);

}

#endif // GENERATOR_H
