#include "Console.h"

#include "framework/Error.h"

#include <iostream>
#include <windows.h>

namespace
{

std::size_t consoleWidth()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return static_cast<std::size_t>(csbi.srWindow.Right - csbi.srWindow.Left + 1);
}

}

std::string banner_imp(std::string title, char ch)
{
    auto width = consoleWidth();

    if(title.empty())
    {
        return std::string(width, '=');
    }

    if(title.length() > width - 8)
    {
        title = title.substr(0, width - 8) + "...";
    }

    return pcx::str(ch, ch, " ", title, " ", std::string(width - (title.length() + 4), ch));
}

std::string elided_imp(std::string text)
{
    auto width = consoleWidth();

    if(text.length() > width)
    {
        return text.substr(0, width - 3) + "...";
    }

    if(text.length() < width)
    {
        text += "\n";
    }

    return text;
}

std::size_t padw(std::size_t n)
{
    return n ? pcx::str(n - 1).length() : 1;
}
