#include <pcx/str.h>
#include <pcx/textfile.h>
#include <pcx/args.h>

#include <cstdlib>
#include <vector>
#include <cctype>

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

    for(auto s: pb)
    {
        if(s.length() > 1 && !std::isspace(s[0]))
        {
            std::cout << "> " << s << "\n";

            std::istringstream is(s);

            std::string exe;
            is >> exe;

            if(std::system(pcx::str("C:\\Projects\\Px\\Px\\build-", exe, "\\release\\", s).c_str()))
            {
                return -1;
            }
        }
    }
}
