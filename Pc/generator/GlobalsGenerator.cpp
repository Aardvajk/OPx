#include "GlobalsGenerator.h"

#include "application/Context.h"

#include "visitors/TypeVisitor.h"

#include "generator/ByteListGenerator.h"

#include "nodes/Node.h"

void GlobalsGenerator::generate(Context &c, std::ostream &os)
{
    for(auto i: c.globals.back())
    {
        os << "var \"" << i.first << "\" = ";

        Visitor::visit<ByteListGenerator>(i.second, c, os);

        os << ";\n";
    }

}
