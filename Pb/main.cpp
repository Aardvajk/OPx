#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <windows.h>

#include <pcx/str.h>

void read(std::vector<std::string> &pb);
void write(const std::vector<std::string> &pb);

int main(int argc, char *argv[])
{
    SetCurrentDirectory("C:/Projects/Px/Px/workspace");

    std::vector<std::string> pb;
    pb.push_back("@echo off\n");

    for(auto s: std::vector<std::string>{ "b", "c", "d", "i", "l", "v" })
    {
        pb.push_back(pcx::str("set path=C:\\Projects\\Px\\Px\\build-P", s, "\\release;%path%"));
    }

    read(pb);
    write(pb);

    std::system("pb_temp.bat");
    std::remove("pb_temp.bat");
}

void read(std::vector<std::string> &pb)
{
    std::ifstream is("pb.bat");

    std::string s;
    while(std::getline(is, s))
    {
        pb.push_back(s);
    }
}

void write(const std::vector<std::string> &pb)
{
    std::ofstream os("pb_temp.bat");
    for(auto s: pb)
    {
        os << s << "\n";
    }
}

