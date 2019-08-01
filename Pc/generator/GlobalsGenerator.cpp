#include "GlobalsGenerator.h"

#include "application/Context.h"

#include "visitors/TypeVisitor.h"

#include "generator/ByteListGenerator.h"

#include "nodes/Node.h"

void GlobalsGenerator::generate(Context &c, std::ostream &os)
{
    for(auto i: c.globals)
    {
        auto sz = c.assertInitSize({ }, TypeVisitor::type(c, i.second));

        os << "var \"" << i.first << "\":" << sz << " = ";

        ByteListGenerator bg(c, os);
        i.second->accept(bg);

        os << ";\n";
    }

}
