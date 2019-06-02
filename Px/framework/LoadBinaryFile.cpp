#include "LoadBinaryFile.h"

#include "framework/Error.h"

#include <fstream>

std::vector<char> loadBinaryFile(const std::string &path)
{
    std::ifstream is(path, std::ios::binary);
    if(!is.is_open())
    {
        throw Error("unable to open - ", path);
    }

    return { std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>() };
}
