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
        if(argc < 2)
        {
            throw Error("no source specified");
        }

        if(argc < 3)
        {
            throw Error("no output specified");
        }

        c.open(argv[1]);

        auto n = Compiler::compile(c);

        std::cout << banner("symbols");
        SymPrinter::print(c.tree.root(), std::cout);

        std::cout << banner("nodes");
        AstPrinter as(std::cout);
        n->accept(as);

        std::cout << banner("generate");
        Generator g(c, std::cout);
        n->accept(g);

        std::string output = argv[2];

        if(true)
        {
            std::ofstream os(output);
            if(!os.is_open())
            {
                throw Error("unable to create - ", output);
            }

            Generator g(c, os);
            n->accept(g);
        }
    }

    catch(const Error &error)
    {
        std::cerr << "pc error";
        if(auto n = error.location())
        {
            std::cerr << " " << c.sources.path(n.id()) << " " << n.line() << "," << n.column();
        }

        std::cerr << ": " << error.what() << "\n";
        return -1;
    }
}

