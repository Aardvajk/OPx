#include "framework/Console.h"

#include <pcx/str.h>
#include <pcx/textfile.h>
#include <pcx/args.h>
#include <pcx/filesys.h>

#include <cstdlib>
#include <vector>
#include <cctype>

std::string exePath(const std::string &exe)
{
    return pcx::str("C:/Projects/Px/Px/build-", exe, "/release/", exe);
}

bool runTest(const std::string &dir, const std::string &path)
{
    auto test = path.substr(0, path.length() - 3);
    auto file = pcx::str(dir, "/", test);

    std::cout << test << "\n";

    if(std::system(pcx::str(exePath("pc"), " -q -I=../lib ", file, ".pc unittest.pi").c_str())) return false;
    if(std::system(pcx::str(exePath("pi"), " -q unittest.pi unittest.po").c_str())) return false;
    if(std::system(pcx::str(exePath("pl"), " -q unittest.px unittest.po ../lib/stdlib.po ../lib/stdtest.po").c_str())) return false;
    if(std::system(pcx::str(exePath("pv"), " -q unittest.px > unittest_result.txt").c_str()))
    {
        std::cout << "pb error: unit test failed\n";
        return false;
    }

    std::vector<std::string> tr;
    pcx::textfile::read("unittest_result.txt", tr);

    std::vector<std::string> cmp;
    if(pcx::textfile::read(pcx::str(file, ".pc.txt"), cmp))
    {
        if(tr.size() != cmp.size())
        {
            std::cout << "pb error: output comparison failed\n";
            return false;
        }

        for(std::size_t i = 0; i < tr.size(); ++i)
        {
            if(tr[i] != cmp[i])
            {
                std::cout << "pb error: output comparison failed\n";
                return false;
            }
        }
    }
    else if(tr.size())
    {
        pcx::textfile::write(pcx::str(file, ".pc.txt"), tr);
    }

    return true;
}

bool runTests(const std::string &dir)
{
    auto scripts = pcx::filesys::find_files(pcx::str(dir, "/*.pc"));

    std::cout << banner("unit tests");
    for(auto &s: scripts)
    {
        if(!runTest(dir, s))
        {
            return false;
        }
    }

    return true;
}

int main(int argc, char *argv[])
{
    std::vector<std::string> files;
    pcx::args args(argc, argv, files);

    if(files.size() < 1)
    {
        std::cerr << "pb error: no batch file specified\n";
        return -1;
    }

    std::vector<std::string> pb;
    pcx::textfile::read(files[0], pb);

    bool echo = true;

    for(auto s: pb)
    {
        if(s.length() > 1 && !std::isspace(s[0]))
        {
            bool quiet = false;

            if(s[0] == '#')
            {
                s = s.substr(1);
                quiet = true;
            }

            if(!quiet && echo)
            {
                std::cout << banner();
                std::cout << "> " << s << "\n";
            }

            std::istringstream is(s);

            std::string exe, param;
            is >> exe >> param;

            if(exe == "echo")
            {
                echo = (param == "on");
            }
            else if(exe == "test")
            {
                if(!runTests(param))
                {
                    return -1;
                }
            }
            else if(std::system(pcx::str("C:/Projects/Px/Px/build-", exe, "/release/", s).c_str()))
            {
                return -1;
            }
        }
    }
}
