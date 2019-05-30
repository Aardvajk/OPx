#include "Console.h"

std::string banner_imp(const std::string &title)
{
    if(title.empty())
    {
        return std::string(120, '=');
    }

    return pcx::str("== ", title, " ", std::string(120 - (title.length() + 4), '='));
}

std::size_t padw(std::size_t n)
{
    return n ? pcx::str(n - 1).length() : 1;
}
