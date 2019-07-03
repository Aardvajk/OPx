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
        if(s.length() < 2 || s.substr(0, 2) != "//")
        {
            std::istringstream is(s);

            std::string exe;
            is >> exe;

            if(!exe.empty())
            {
                if(std::system(pcx::str("C:\\Projects\\Px\\Px\\build-", exe, "\\release\\", s).c_str()))
                {
                    return -1;
                }
            }
        }
    }
}
