#include "framework/Error.h"
#include "framework/Console.h"

#include "application/Context.h"

#include "symbols/SymPrinter.h"

#include "compiler/Compiler.h"

#include "visitors/AstPrinter.h"

#include "generator/Generator.h"

#include "tests/Tests.h"

#include <fstream>

int main(int argc, char *argv[])
{
    if(!runTests()) return -1;

    Context c;

    try
    {
        c.open("C:/Projects/Px/Px/Pc/script.txt");

        auto n = Compiler::compile(c);

        std::cout << banner("symbols");
        SymPrinter::print(c.tree.root(), std::cout);

        std::cout << banner("nodes");
        AstPrinter as(std::cout);
        n->accept(as);

        std::cout << banner("generate");
        Generator g(c, std::cout);
        n->accept(g);

        if(true)
        {
            std::ofstream os("C:/Projects/Px/Px/script.pi");
            if(!os.is_open())
            {
                throw Error("unable to create - script.pi");
            }

            Generator g(c, os);
            n->accept(g);
        }

        checked_system("C:/Projects/Px/Px/build-Pi/release/pi C:/Projects/Px/Px/script.pi C:/Projects/Px/Px/script.po");
    }

    catch(const Error &error)
    {
        std::cerr << "pc error";
        if(auto n = error.location())
        {
            std::cerr << " " << c.sources.path(n.id()) << " " << n.line() << "," << n.column();
        }

        std::cerr << ": " << error.what() << "\n";
    }
}

