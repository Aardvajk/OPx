#include "Console.h"

#include "framework/Error.h"

#include <iostream>

std::string banner_imp(std::string title, char ch)
{
    if(title.empty())
    {
        return std::string(120, '=');
    }

    if(title.length() > 112)
    {
        title = title.substr(0, 112) + "...";
    }

    return pcx::str(ch, ch, " ", title, " ", std::string(120 - (title.length() + 4), ch));
}

std::string elided_imp(std::string text)
{
    if(text.length() > 120)
    {
        return text.substr(0, 117) + "...";
    }

    if(text.length() < 120)
    {
        text += "\n";
    }

    return text;
}

std::size_t padw(std::size_t n)
{
    return n ? pcx::str(n - 1).length() : 1;
}
