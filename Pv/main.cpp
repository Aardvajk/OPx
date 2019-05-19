#include "application/Machine.h"

#include <iostream>
#include <fstream>
#include <vector>

std::vector<char> readFile(const std::string &path)
{
    std::ifstream file(path, std::ios::binary);
    return std::vector<char>(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

int main(int argc, char *argv[])
{
//    std::ofstream os("z.dat", std::ios::binary);
//    char c = '1';
//    os.write(&c, sizeof(char));

//    c = '2';
//    os.write(&c, sizeof(char));

//    c = '\0';
//    os.write(&c, sizeof(char));

//    c = '2';
//    os.write(&c, sizeof(char));

//    c = '\0';
//    os.write(&c, sizeof(char));

//    c = 'z';
//    os.write(&c, sizeof(char));

    auto f = readFile("z.dat");

    for(auto c: f) std::cout << c << "\n";
}
