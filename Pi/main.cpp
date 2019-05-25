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

        std::cout << banner("functions");

        for(auto &f: c.funcs)
        {
            std::cout << "func " << f.sym->name << "\n";
        }

        if(true)
        {
            std::ofstream os("C:/Projects/Px/Px/script.po", std::ios::binary);
            ByteStream bs(os);

            bs << c.strings.size();
            for(auto &s: c.strings)
            {
                bs << s;
            }

            bs << c.funcs.size();
            for(auto &f: c.funcs)
            {
                ByteStreamPatch p;

                bs << 'F' << f.id;

                bs << p;
                auto i = bs.position();

                auto bytes = f.bytes.data();

                bs.write(bytes.data(), bytes.size());
                p.patch(bs, bs.position() - i);
            }
        }

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
