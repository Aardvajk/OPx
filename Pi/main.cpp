#include "error/Error.h"

#include "application/Context.h"

#include "compiler/Compiler.h"

#include <pcx/str.h>

std::string banner(const std::string &title);

int main(int argc, char *argv[])
{
    Context c;

    try
    {
        c.open("C:/Projects/Px/Px/Pi/script.txt");

        std::cout << banner("");

        compile(c);

        std::cout << banner("");

        c.syms.print(std::cout);

        std::cout << banner("");

        for(auto &f: c.funcs)
        {
            std::cout << "func: " << f.str().size() << "\n";
        }

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

