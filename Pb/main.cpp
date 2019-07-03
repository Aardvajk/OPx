#include <pcx/str.h>
#include <pcx/textfile.h>

#include <cstdlib>
#include <vector>
#include <cctype>

int main(int argc, char *argv[])
{
    std::vector<std::string> pb;
    pcx::textfile::read("pb.bat", pb);

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
