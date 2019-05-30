#ifndef CONSOLE_H
#define CONSOLE_H

#include <pcx/str.h>

std::string banner_imp(const std::string &title);

template<typename... Args> std::string banner(Args&&... args)
{
    return banner_imp(pcx::str(std::forward<Args>(args)...));
}

std::size_t padw(std::size_t n);

template<typename T> std::string pad(const T &text, std::size_t width)
{
    auto s = pcx::str(text);
    while(s.length() < width)
    {
        s = ' ' + s;
    }

    return s;
}

#endif // CONSOLE_H
