#include "TextFile.h"

#include <fstream>

TextFile::TextFile(const std::string &path) : ff(true), p(path)
{
    std::ifstream is(path);
    if(is.is_open())
    {
        ff = false;

        std::string s;
        while(std::getline(is, s))
        {
            v.push_back(s);
        }
    }
}
