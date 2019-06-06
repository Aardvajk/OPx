#ifndef CONSOLE_H
#define CONSOLE_H

#include <pcx/str.h>

#include <vector>

std::string banner_imp(std::string title, char ch);

template<typename... Args> std::string banner(Args&&... args)
{
    return banner_imp(pcx::str(std::forward<Args>(args)...), '=');
}

template<typename... Args> std::string custom_banner(char ch, Args&&... args)
{
    return banner_imp(pcx::str(std::forward<Args>(args)...), ch);
}

std::string elided_imp(std::string text);

template<typename... Args> std::string elided(Args&&... args)
{
    return elided_imp(pcx::str(std::forward<Args>(args)...));
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

void checked_system(const std::string &command);

#endif // CONSOLE_H
