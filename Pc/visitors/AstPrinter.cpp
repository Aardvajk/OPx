#include "AstPrinter.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/DotNode.h"
#include "nodes/ClassNode.h"

#include "symbols/Sym.h"

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

void AstPrinter::visit(IdNode &node)
{
    tab() << "id " << node.name << "\n";
}

void AstPrinter::visit(DotNode &node)
{
    tab() << "dot " << node.name << "\n";
    if(node.child)
    {
        auto g = pcx::scoped_counter(tc);
        node.child->accept(*this);
    }
}

void AstPrinter::visit(ClassNode &node)
{
    tab() << "class " << node.sym->fullname() << "\n";
    node.block->accept(*this);
}

std::ostream &AstPrinter::tab() const
{
    return os << std::string(tc * 4, ' ');
}
