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
        auto n = compile(c);

        std::cout << "== symbols ===============================================\n";

        c.tree.root()->print(std::cout);

        std::cout << "== nodes =================================================\n";

        AstPrinter pr(std::cout);
        n->accept(pr);

        std::cout << "==========================================================\n";
    }

    catch(const Error &error)
    {
        std::cout << "error";
        if(auto n = error.location())
        {
            std::cout << " " << c.sources.path(n.id()) << " " << n.column() << "," << n.line();
        }

        std::cout << ": " << error.what() << "\n";
    }
}
