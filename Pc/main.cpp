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
    Context c;

    try
    {
        bool quiet = false;

        int arg = 0;
        if(argc > 1 && std::string(argv[1]) == "-q")
        {
            quiet = true;
            ++arg;
        }

        if(argc < 2 + arg)
        {
            throw Error("no source specified");
        }

        if(argc < 3 + arg)
        {
            throw Error("no output specified");
        }

        c.open(argv[arg + 1]);

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
        }

        visit<Transformer>(n, c);

        if(!quiet)
        {
            std::cout << banner("transformed nodes");
            visit<AstPrinter>(n, std::cout);

            std::cout << banner("symbols");
            SymPrinter::print(c.tree.root(), std::cout);
        }

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
            std::ofstream os(argv[arg + 2]);
            if(!os.is_open())
            {
                throw Error("unable to create - ", argv[arg + 2]);
            }

            generate(c, os, n);
        }

        if(!quiet)
        {
            std::cout << banner();
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
