#ifndef GLOBALFUNCS_H
#define GLOBALFUNCS_H

#include <cstdlib>

class Context;

namespace GlobalFuncs
{

std::size_t generateGlobalInit(Context &c, std::size_t entryPointOffset);

}

#endif // GLOBALFUNCS_H
