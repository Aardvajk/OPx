#include "ObjectDisassembler.h"

#include "framework/Console.h"

#include "application/Context.h"

#include <pcx/str.h>

ObjectDisassembler::ObjectDisassembler(std::size_t index) : index(index), i(0)
{
}

void ObjectDisassembler::map(Context &c, std::size_t pc)
{
    if(index < c.dm.size())
    {
        auto &cm = c.dm[index].comments;
        while(i < cm.size() && cm[i].address == pc)
        {
            auto &s = cm[i].text;

            std::cout << (s[0] == '-' ? pcx::str("-- ", s.substr(1), " ", std::string(120 - (s.length() + 3), '-')) : pcx::str(s, "\n"));
            ++i;
        }
    }
}
