#include "framework/Error.h"
#include "framework/Console.h"

#include "application/Context.h"

#include "compiler/Compiler.h"

#include "visitors/AstPrinter.h"

#include "decorator/Decorator.h"

#include "generator/Generator.h"

#include "syms/SymPrinter.h"

#include <fstream>

template<typename T, typename... Args> void visit(NodePtr &node, Args&&... args)
{
    T v(std::forward<Args>(args)...);
    node->accept(v);
}

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
        visit<AstPrinter>(n, std::cout);

        visit<Decorator>(n, c);

        std::cout << banner("decorated nodes");
        visit<AstPrinter>(n, std::cout);

        std::cout << banner("symbols");
        SymPrinter::print(c.tree.root(), std::cout);

        std::cout << banner("generate");
        visit<Generator>(n, c, std::cout);

        if(true)
        {
            std::ofstream os(argv[2]);
            if(!os.is_open())
            {
                throw Error("unable to create - ", argv[2]);
            }

            visit<Generator>(n, c, os);
        }

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
