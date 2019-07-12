#include "framework/Error.h"
#include "framework/Console.h"

#include "application/Context.h"

#include "compiler/Compiler.h"

#include "visitors/AstPrinter.h"

#include "generator/Generator.h"

#include "syms/SymPrinter.h"

#include <iostream>

int main(int argc, char *argv[])
{
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

        std::cout << banner("nodes");
        AstPrinter ap(std::cout);
        n->accept(ap);

        Generator gn(c, std::cout);
        n->accept(gn);

        std::cout << banner("symbols");
        SymPrinter::print(c.tree.root(), std::cout);

        std::cout << banner();
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
