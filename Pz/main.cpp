#include "framework/Error.h"
#include "framework/Console.h"

#include "application/Context.h"

#include "parser/Parser.h"

#include "decorator/Decorator.h"

#include "finaliser/Finaliser.h"

#include "generator/Generator.h"

#include "visitors/AstPrinter.h"

#include "syms/SymPrinter.h"

#include "types/Type.h"
#include "types/LowerTypes.h"

#include <iostream>

int main(int argc, char *argv[])
{
    std::vector<std::string> files;
    Context c(argc, argv, files);

    try
    {
        if(files.size() < 1)
        {
            throw Error("no source specified");
        }

        c.open(files[0]);

        auto n = Parser::build(c);

        if(!c.option("q"))
        {
            std::cout << banner("nodes");
            Visitor::visit<AstPrinter>(n.get(), std::cout);
        }

        Visitor::visit<Decorator>(n.get(), c);

        if(!c.option("q"))
        {
            std::cout << banner("decorated nodes");
            Visitor::visit<AstPrinter>(n.get(), std::cout);

            std::cout << banner("symbols");
            SymPrinter::print(c.tree.root(), std::cout);
        }

        LowerTypes::lower(c);

        if(!c.option("q"))
        {
            std::cout << banner("lowered nodes");
            Visitor::visit<AstPrinter>(n.get(), std::cout);

            std::cout << banner("lowered symbols");
            SymPrinter::print(c.tree.root(), std::cout);
        }

        Visitor::visit<Finaliser>(n.get(), c);

        if(!c.option("q"))
        {
            std::cout << banner("finalised symbols");
            SymPrinter::print(c.tree.root(), std::cout);
        }

        if(!c.option("q"))
        {
            std::cout << banner("generated");
            Visitor::visit<Generator>(n.get(), c, std::cout);
        }

        if(!c.option("q"))
        {
            std::cout << banner();
        }
    }

    catch(const Error &error)
    {
        std::cerr << "pz error";
        if(auto n = error.location())
        {
            std::cerr << " " << c.sources.path(n.id()) << " " << n.line() << "," << n.column();
        }

        std::cerr << ": " << error.what() << "\n";
        return -1;
    }
}
