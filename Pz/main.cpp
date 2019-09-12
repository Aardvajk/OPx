#include "framework/Error.h"
#include "framework/Console.h"

#include "application/Context.h"

#include "parser/Parser.h"

#include "visitors/AstPrinter.h"

#include <iostream>

int main(int argc, char *argv[])
{
    std::vector<std::string> files;
    Context c(argc, argv, files);

    try
    {
        auto quiet = c.option("q");

        if(files.size() < 1)
        {
            throw Error("no source specified");
        }

        c.open(files[0]);

        auto n = Parser::build(c);

        n->property<int>("foo");

        if(!quiet)
        {
            std::cout << banner("nodes");
            Visitor::visit<AstPrinter>(n.get(), std::cout);
        }

        if(!quiet)
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
