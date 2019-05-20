#include <iostream>
#include <fstream>
#include <vector>

#include "application/Machine.h"
#include "application/Error.h"

#include <cstring>
#include <sstream>

//std::vector<char> readFile(const std::string &path)
//{
//    std::ifstream file(path, std::ios::binary);
//    return std::vector<char>(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
//}

class Stream
{
public:
    Stream() : os(std::ios::binary) { }

    template<typename T> Stream &operator<<(const T &t){ os.write(reinterpret_cast<const char*>(&t), sizeof(T)); return *this; }
    std::vector<char> data(){ auto s = os.str(); return std::vector<char>(s.begin(), s.end()); }

private:
    std::ostringstream os;
};

int main(int argc, char *argv[])
{
    try
    {
        Stream s;

        if(true)
        {
            using namespace OpCode;

            s << Type::PrintR << Reg::Bp;

            s << Type::SubRI << Reg::Sp << std::size_t(8);
            s << Type::CopyRA << Reg::Bp << Reg::Sp;

            s << Type::CopyRR << Reg::Pc << Reg::Bp;
            s << Type::PrintR << Reg::Bp;

            s << Type::CopyAR << Reg::Sp << Reg::Bp;
            s << Type::PrintR << Reg::Bp;

            s << Type::AddRI << Reg::Sp << std::size_t(8);
            s << Type::End;
        }

        Machine m(s.data());
        m.execute();
    }

    catch(const Error &e)
    {
        std::cerr << "error: " << e.what() << "\n";
        return -1;
    }
}
