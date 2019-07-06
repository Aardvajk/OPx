#include "AstPrinter.h"

#include "nodes/BlockNode.h"

#include <pcx/scoped_counter.h>

#include <iostream>

AstPrinter::AstPrinter(std::ostream &os) : os(os), tc(0)
{
}

void AstPrinter::visit(BlockNode &node)
{
    tab() << "block\n";
    tab() << "{\n";

    if(true)
    {
        auto g = pcx::scoped_counter(tc);
        for(auto &n: node.nodes)
        {
            n.accept(*this);
        }
    }

    tab() << "}\n";
}

std::ostream &AstPrinter::tab() const
{
    return os << std::string(tc * 4, ' ');
}
