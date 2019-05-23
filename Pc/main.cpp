#include "framework/Error.h"

#include "application/Context.h"

#include "symbols/SymPrinter.h"

#include "compiler/Compiler.h"

#include "visitors/AstPrinter.h"

#include "tests/Tests.h"

#include <pcx/str.h>

std::string banner(const std::string &title);

int main(int argc, char *argv[])
{
    if(!runTests()) return -1;

    Context c;

    try
    {
        c.open("C:/Projects/Px/Px/Pc/script.txt");

        std::cout << banner("");

        auto n = Compiler::compile(c);

        std::cout << banner("symbols");
        SymPrinter::print(c.tree.root(), std::cout);

        std::cout << banner("nodes");
        AstPrinter as(std::cout);
        n->accept(as);

        std::cout << banner("");
    }

    catch(const Error &error)
    {
        std::cerr << "error";
        if(auto n = error.location())
        {
            std::cerr << " " << c.sources.path(n.id()) << " " << n.line() << "," << n.column();
        }

        std::cerr << ": " << error.what() << "\n";
    }
}

std::string banner(const std::string &title)
{
    if(title.empty())
    {
        return std::string(120, '=');
    }

    return pcx::str("== ", title, " ", std::string(120 - (title.length() + 4), '='));
}

