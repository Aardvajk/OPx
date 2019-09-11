#include "framework/Error.h"
#include "framework/Console.h"

#include "application/Context.h"

#include "compiler/Compiler.h"

#include "visitors/AstPrinter.h"
#include "visitors/TypeVisitor.h"

#include "transform/Transformer.h"

#include "decorator/Decorator.h"

#include "lower/Lower.h"

#include "generator/Generator.h"
#include "generator/GlobalsGenerator.h"

#include "syms/SymPrinter.h"

#include "types/Type.h"

#include <pcx/args.h>

#include <fstream>

template<typename T, typename... Args> void visit(NodePtr &node, Args&&... args)
{
    T v(std::forward<Args>(args)...);
    node->accept(v);
}

void generate(Context &c, std::ostream &os, NodePtr &n)
{
    GlobalsGenerator::generate(c, os);
    visit<Generator>(n, c, os);
}

int main(int argc, char *argv[])
{
    std::vector<std::string> files;
    Context c(argc, argv, files);

    try
    {
        bool quiet = c.args.back().contains("q");

        if(files.size() < 1)
        {
            throw Error("no source specified");
        }

        if(files.size() < 2)
        {
            throw Error("no output specified");
        }

        c.open(files[0]);

        auto n = Compiler::compile(c);

        if(!quiet)
        {
            std::cout << banner("nodes");
            visit<AstPrinter>(n, std::cout);
        }

        visit<Decorator>(n, c);

        if(!quiet)
        {
            std::cout << banner("decorated nodes");
            visit<AstPrinter>(n, std::cout);

            std::cout << banner("symbols");
            SymPrinter::print(c.tree.root(), std::cout);
        }

        visit<Transformer>(n, c);

        if(!quiet)
        {
            std::cout << banner("transformed nodes");
            visit<AstPrinter>(n, std::cout);
        }

        for(auto &t: c.types)
        {
            if(t.ref)
            {
                ++t.ptr;
            }
        }

        c.refsLowered = true;

        visit<Lower>(n, c);

        if(!quiet)
        {
            std::cout << banner("lowered nodes");
            visit<AstPrinter>(n, std::cout);

            std::cout << banner("lowered symbols");
            SymPrinter::print(c.tree.root(), std::cout);
        }

        if(!quiet)
        {
            std::cout << banner("generate");
            generate(c, std::cout, n);
        }

        if(true)
        {
            std::ofstream os(files[1]);
            if(!os.is_open())
            {
                throw Error("unable to create - ", files[1]);
            }

            generate(c, os, n);
        }

        if(!c.args.back()["test_error"].empty())
        {
            throw Error("error expected - ", c.args.back()["test_error"].front());
        }

        if(c.args.back().contains("test"))
        {
            if(std::system(pcx::str("C:/Projects/Px/Px/build-pi/release/pi -q script.pi script.po").c_str())) return -1;
            if(std::system(pcx::str("C:/Projects/Px/Px/build-pl/release/pl -q script.pv script.po foo.po ../lib/stdlib.po ../lib/stdios.po").c_str())) return -1;
            if(std::system(pcx::str("C:/Projects/Px/Px/build-pv/release/pv script.pv").c_str())) return -1;
        }
    }

    catch(const Error &error)
    {
        if(c.option("test_error", error.what()))
        {
            return 100;
        }

        std::cerr << "pc error";
        if(auto n = error.location())
        {
            std::cerr << " " << c.sources.path(n.id()) << " " << n.line() << "," << n.column();
        }

        std::cerr << ": " << error.what() << "\n";
        return -1;
    }
}
