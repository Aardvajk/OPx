#ifndef QUAL_H
#define QUAL_H

#include <pcx/flags.h>

namespace Qual
{

enum class Flag
{
    Free = 1,
    External = 2,
    Explicit = 4
};

using Flags = pcx::flags<Flag>;

}

template<> struct pcx_is_flag_enum<Qual::Flag> : std::true_type { };

#endif // QUAL_H
