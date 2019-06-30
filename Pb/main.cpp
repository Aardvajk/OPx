#include <cstdlib>
#include <vector>

#include <pcx/str.h>
#include <pcx/textfile.h>

int main(int argc, char *argv[])
{
    std::vector<std::string> pb;
    pb.push_back("@echo off\n");

    for(auto s: std::vector<std::string>{ "b", "c", "d", "i", "l", "v" })
    {
        pb.push_back(pcx::str("set path=C:\\Projects\\Px\\Px\\build-P", s, "\\release;%path%"));
    }

    pcx::textfile::read("pb.bat", pb);
    pcx::textfile::write("pb_temp.bat", pb);

    std::system("pb_temp.bat");
    std::remove("pb_temp.bat");
}
