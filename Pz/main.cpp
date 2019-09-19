#include "framework/Error.h"
#include "framework/Console.h"

#include "application/Context.h"

#include "parser/Parser.h"

#include "decorator/Decorator.h"

#include "transform/Transform.h"

#include "lower/Lower.h"
#include "lower/LowerTypes.h"

#include "finaliser/Finaliser.h"

#include "generator/Generator.h"

#include "visitors/AstPrinter.h"

#include "syms/SymPrinter.h"

#include "types/Type.h"

#include <iostream>
#include <fstream>

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

        if(files.size() < 2)
        {
            throw Error("no output specified");
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

        Visitor::visit<Transform>(n.get(), c);

        if(!c.option("q"))
        {
            std::cout << banner("transformed nodes");
            Visitor::visit<AstPrinter>(n.get(), std::cout);
        }

        LowerTypes::convertRefsToPtrs(c);
        Visitor::visit<Lower>(n.get(), c);
        LowerTypes::removeRefs(c);

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

        if(true)
        {
            std::ofstream os(files[1], std::ios::binary);
            if(!os.is_open())
            {
                throw Error("unable to create - ", files[1]);
            }

            Visitor::visit<Generator>(n.get(), c, os);
        }

        if(c.args.back().contains("test"))
        {
            if(std::system(pcx::str("C:/Projects/Px/Px/build-pi/release/pi -q script.pi script.po").c_str())) return -1;
            if(std::system(pcx::str("C:/Projects/Px/Px/build-pl/release/pl -q script.pv script.po ../lib/stdlib.po ../lib/stdios.po").c_str())) return -1;
            if(std::system(pcx::str("C:/Projects/Px/Px/build-pv/release/pv script.pv").c_str())) return -1;
        }
        else if(!c.option("q"))
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
