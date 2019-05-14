#include "application/Context.h"
#include "application/Error.h"

#include "compiler/Compiler.h"

#include "visitors/AstPrinter.h"

#include <pcx/str.h>

std::string banner(const std::string &title);

int main()
{
    Context c;

    try
    {
        c.open("C:/Projects/Px/Px/Pc/script.txt");

        std::cout << banner("");

        compile(c);

        std::cout << banner("symbols");
        c.tree.root()->print(std::cout);

        std::cout << banner("");
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

std::string banner(const std::string &title)
{
    if(title.empty())
    {
        return std::string(120, '=');
    }

    return pcx::str("== ", title, " ", std::string(120 - (title.length() + 4), '='));
}

