#include "Test.h"

#include "application/TextFile.h"

#include <pcx/str.h>
#include <pcx/optional.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

namespace
{

bool createInput(const std::string &path, const TextFile &file)
{
    std::ofstream os(path);
    if(!os.is_open())
    {
        std::cerr << "pt error: unable to create - " << path << "\n";
        return false;
    }

    for(std::size_t i = 2; i < file.size(); ++i)
    {
        os << file[i] << "\n";
    }

    return true;
}

std::string formatString(const std::string &s)
{
    std::ostringstream os;
    for(std::size_t i = 0; i < s.size(); ++i)
    {
        auto ch = s[i];

        if(ch == '\\')
        {
            if(i < s.size() - 1)
            {
                ch = s[++i];

                switch(ch)
                {
                    case 'n': ch = '\n'; break;

                    default: break;
                }
            }
        }

        os << ch;
    }

    return os.str();
}

pcx::optional<std::string> createResult(const std::string &path)
{
    std::ostringstream os;

    std::ifstream is(path);
    if(!is.is_open())
    {
        return { };
    }

    std::string s;
    while(std::getline(is, s))
    {
        os << s << "\n";
    }

    return os.str();
}

}

bool runTest(const std::string &cmd, const std::string &dir, const TextFile &file)
{
    auto input = pcx::str(dir, "/pt_input.txt");
    auto output = pcx::str(dir, "/pt_output.txt");

    if(!createInput(input, file))
    {
        return false;
    }

    std::system(pcx::str(cmd, " ", input, " > ", output).c_str());

    auto expected = formatString(file[0]);

    auto result = createResult(output);

    if(!result)
    {
        std::cerr << "unable to open - " << output << "\n";
        return false;
    }

    if(expected != *result)
    {
        std::cerr << "pt failed: " << file.path() << "\n";
        std::cerr << "expected:\n" << expected << "\n";
        std::cerr << "result:\n" << *result << "\n";

        return false;
    }

    std::cout << "pt passed: " << file.path() << "\n";
    return true;
}
