#include "Tests.h"

#include "application/Context.h"
#include "application/Error.h"

#include "compiler/Compiler.h"

#include <pcx/str.h>

namespace
{

bool runTest(const std::string &script)
{
    Context c;

    try
    {
        c.open(pcx::str("C:/Projects/Px/Px/Pc/tests/scripts/", script, ".txt"));
        compile(c);

        return true;
    }

    catch(const Error &error)
    {
        auto n = error.location();
        std::cerr << "test error " << script << " " << n.line() << "," << n.column() << ": " << error.what() << "\n";

        return false;
    }
}

}

bool runTests()
{
    const std::vector<std::string> scripts =
    {
        "lookup"
    };

    for(auto script: scripts)
    {
        if(!runTest(script))
        {
            return false;
        }
    }

    return true;
}
