#include <cstdlib>
#include <vector>

#include <pcx/str.h>
#include <pcx/textfile.h>

int main(int argc, char *argv[])
{
    std::vector<std::string> pb;
    pcx::textfile::read("pb.bat", pb);

    for(auto s: pb)
    {
        std::istringstream is(s);

        std::string exe;
        is >> exe;

        auto cmd = pcx::str("C:\\Projects\\Px\\Px\\build-", exe, "\\release\\", s);

        if(std::system(cmd.c_str()))
        {
            return -1;
        }
    }
}
