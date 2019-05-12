#include "application/Context.h"
#include "application/Error.h"

#include "compiler/Compiler.h"

#include "visitors/AstPrinter.h"

int main()
{
    Context c;

    try
    {
        c.open("C:/Projects/Px/Px/Pc/script.txt");

        std::cout << "==========================================================\n";

        compile(c);

        std::cout << "== symbols ===============================================\n";

        c.tree.root()->print(std::cout);

        std::cout << "==========================================================\n";
    }

    catch(const Error &error)
    {
        std::cout << "error";
        if(auto n = error.location())
        {
            std::cout << " " << c.sources.path(n.id()) << " " << n.line() << "," << n.column();
        }

        std::cout << ": " << error.what() << "\n";
    }
}
