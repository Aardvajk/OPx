#include "framework/Error.h"
#include "framework/Console.h"

#include "application/Context.h"

#include "parser/Parser.h"

#include "decorator/Decorator.h"

#include "convert/Convert.h"

#include "transform/Transform.h"

#include "lower/Lower.h"
#include "lower/LowerTypes.h"

#include "finaliser/Finaliser.h"

#include "generator/Generator.h"
#include "generator/GlobalsGenerator.h"

#include "visitors/AstPrinter.h"

#include "syms/SymPrinter.h"

#include "types/Type.h"

#include <pcx/textfile.h>
#include <pcx/scoped_push.h>

#include <iostream>
#include <fstream>

#include <initializer_list>

int main(int argc, char *argv[])
{
    std::vector<std::string> files;
    Context c(argc, argv, files);

    try
    {
        if(c.option("config"))
        {
            if(!c.args.back().readFromFile(c.values("config").front()))
            {
                throw Error("unable to open config - ", c.values("config").front());
            }
        }

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

auto cn = n->clone();
n = { };

n = cn;

        if(!c.option("q"))
        {
            std::cout << banner("nodes");
            Visitor::visit<AstPrinter>(n.get(), c, std::cout);
        }

        Decorator::decorate(c, n);

        if(!c.option("q"))
        {
            std::cout << banner("symbols");
            SymPrinter::print(c, c.tree.root(), std::cout);

            std::cout << banner("decorated nodes");
            Visitor::visit<AstPrinter>(n.get(), c, std::cout);
        }

        Visitor::visit<Convert>(n.get(), c);

        if(!c.option("q"))
        {
            std::cout << banner("converted nodes");
            Visitor::visit<AstPrinter>(n.get(), c, std::cout);
        }

        Visitor::visit<Transform>(n.get(), c);

        if(!c.option("q"))
        {
            std::cout << banner("transformed nodes");
            Visitor::visit<AstPrinter>(n.get(), c, std::cout);
        }

        LowerTypes::convertRefsToPtrs(c);
        Visitor::visit<Lower>(n.get(), c);
        LowerTypes::removeRefs(c);

        if(!c.option("q"))
        {
            std::cout << banner("lowered nodes");
            Visitor::visit<AstPrinter>(n.get(), c, std::cout);
        }

        Visitor::visit<Finaliser>(n.get(), c);

        if(!c.option("q"))
        {
            std::cout << banner("finalised nodes");
            Visitor::visit<AstPrinter>(n.get(), c, std::cout);
        }

        if(!c.option("q"))
        {
            std::cout << banner("generated");

            GlobalsGenerator::generate(c, std::cout);
            Visitor::visit<Generator>(n.get(), c, std::cout);
        }

        if(true)
        {
            std::ofstream os(files[1], std::ios::binary);
            if(!os.is_open())
            {
                throw Error("unable to create - ", files[1]);
            }

            auto sg = pcx::scoped_push(c.args, c.args.back());
            c.args.back().process("debug-=suppress_autogens");

            GlobalsGenerator::generate(c, os);
            Visitor::visit<Generator>(n.get(), c, os);
        }

        if(c.option("test_error"))
        {
            throw Error("error expected - ", c.values("test_error").front());
        }

        if(c.option("pb"))
        {
            if(std::system(pcx::str("C:/Projects/Px/Px/build-pb/release/pb ", c.values("pb").front()).c_str())) return -1;
        }
        else if(!c.option("q"))
        {
            std::cout << banner();
        }
    }

    catch(const Error &error)
    {
        if(c.option("test_error", error.what()))
        {
            return 100;
        }

        std::cerr << "pc error";

        std::string source;
        unsigned col = 0;

        if(auto n = error.location())
        {
            std::vector<std::string> v;

            if(c.option("descriptive_errors"))
            {
                pcx::textfile::read(c.sources.path(n.id()), v);
            }

            std::cerr << " " << c.sources.path(n.id()) << " " << n.line() << "," << n.column();

            if(n.line() - 1 < v.size())
            {
                source = v[n.line() - 1];
                col = n.column();
            }
        }

        std::cerr << ": " << error.what() << "\n";

        if(!source.empty())
        {
            std::cerr << source << "\n";
            std::cerr << std::string(col - 1, ' ') << "^\n";
        }

        return -1;
    }
}
