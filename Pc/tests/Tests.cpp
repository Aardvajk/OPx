#include "Tests.h"

#include "application/Context.h"
#include "application/Error.h"

#include "compiler/Compiler.h"

#include <pcx/str.h>

#include <windows.h>

namespace
{

bool runTest(const std::string &script)
{
    std::cout << "running test: " << script << "\n";

    Context c;

    try
    {
        c.open(pcx::str("C:/Projects/Px/Px/Pc/tests/scripts/", script, ".txt"));
        Compiler::compile(c);

        return true;
    }

    catch(const Error &error)
    {
        auto n = error.location();
        std::cerr << "test error " << script << " " << n.line() << "," << n.column() << ": " << error.what() << "\n";

        return false;
    }
}

}

bool runTests()
{
    std::cout << "== tests " << std::string(111, '=');

    std::vector<std::string> scripts;

    WIN32_FIND_DATA fd;
    HANDLE hFind = FindFirstFile("C:/Projects/Px/Px/Pc/tests/scripts/*.txt", &fd);

    if(hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                std::string s(fd.cFileName);
                scripts.push_back(s.substr(0, s.length() - 4));
            }
        }
        while(FindNextFile(hFind, &fd));

        FindClose(hFind);
    }

    for(auto script: scripts)
    {
        if(!runTest(script))
        {
            return false;
        }
    }

    return true;
}
