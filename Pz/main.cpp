#include "framework/Error.h"
#include "framework/Console.h"

#include "application/Context.h"

#include "parser/Parser.h"

#include "decorate/Decorator.h"

#include "visitors/AstPrinter.h"

#include "syms/SymPrinter.h"

#include <iostream>

int main(int argc, char *argv[])
{
    std::vector<std::string> files;
    Context c(argc, argv, files);

    try
    {
        if(files.size() < 2)
        {
            throw Error("no files specified");
        }

        c.open(files[0]);

        auto n = Parser::build(c);

        if(!c.args.contains("q"))
        {
            std::cout << banner("nodes");
            Visitor::visit<AstPrinter>(n.get(), c, std::cout);
        }

        Visitor::visit<Decorator>(n.get(), c);

        if(!c.args.contains("q"))
        {
            std::cout << banner("symbols");
            SymPrinter::print(c, c.tree.root(), std::cout);

            std::cout << banner("decorated nodes");
            Visitor::visit<AstPrinter>(n.get(), c, std::cout);
        }

        if(!c.args.contains("q"))
        {
            std::cout << banner();
        }
    }

    catch(const Error &e)
    {
        std::cerr << "pz error: " << e.what() << "\n";
        return -1;
    }
}
