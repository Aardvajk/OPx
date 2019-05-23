#include "application/Context.h"

#include "framework/Error.h"
#include "framework/Console.h"

#include "compiler/Compiler.h"

#include <pcx/str.h>

#include "common/OpCode.h"
#include <fstream>

int main(int argc, char *argv[])
{
    Context c;

    try
    {
        c.open("C:/Projects/Px/Px/Pi/script.txt");

        compile(c);

        std::cout << banner("symbols");

        c.syms.print(std::cout);

        std::cout << banner("");

        for(auto &f: c.funcs)
        {
            std::cout << "func " << f.sym->name << "\n";
        }

        if(true)
        {
            std::ofstream os("C:/Projects/Px/Px/script.po", std::ios::binary);
            ByteStream bs(os);

            ByteStreamPatch p;

            bs << std::size_t(2);
            bs << "main()";
            bs << "foo()";

            bs << std::size_t(2);

            bs << 'F' << std::size_t(0);

            bs << p;
            auto i = bs.position();

            bs << OpCode::Type::Int << 1010;
            bs << OpCode::Type::Ret << std::size_t(0);
            p.patch(bs, bs.position() - i);

            bs << 'F' << std::size_t(1);

            bs << p;
            i = bs.position();

            bs << OpCode::Type::Int << 1020;
            bs << OpCode::Type::Int << 1030;
            bs << OpCode::Type::Ret << std::size_t(0);

            p.patch(bs, bs.position() - i);
        }

        std::cout << banner("launching pl");
        std::system("C:/Projects/Px/Px/build-Pl/release/pl C:/Projects/Px/Px/script.po");
    }

    catch(const Error &error)
    {
        std::cerr << "error";
        if(auto n = error.location())
        {
            std::cerr << " " << c.sources.path(n.id()) << " " << n.line() << "," << n.column();
        }

        std::cerr << ": " << error.what() << "\n";
    }
}
