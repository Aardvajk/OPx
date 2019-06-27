#include "ExprGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/IntLiteralNode.h"
#include "nodes/CallNode.h"

#include "visitors/NameVisitors.h"
#include "visitors/TypeVisitor.h"

ExprGenerator::ExprGenerator(Context &c, std::ostream &os) : c(c), os(os), sz(0)
{
}

void ExprGenerator::visit(IntLiteralNode &node)
{
    os << "    push int(" << node.value << ");\n";
    sz = sizeof(int);
}

void ExprGenerator::visit(CallNode &node)
{
    Type searchType;
    for(auto &a: node.args)
    {
        TypeVisitor tv(c);
        a.accept(tv);

        if(!tv.result())
        {
            throw Error(a.location(), "type not detectable - ", NameVisitors::prettyName(&a));
        }

        searchType.args.push_back(tv.result()->clone());
    }

    auto sym = c.matchFunction(node.target->location(), node.target->property("syms").value<std::vector<SymResult> >(), &searchType);
    if(!sym)
    {
        throw Error(node.location(), "no matching function - ", NameVisitors::prettyName(node.target.get()));
    }

    auto type = sym->property("type").to<const Type*>();

    sz = c.assertSize(node.target->location(), type->returnType.get());

    os << "    allocs " << sz << ";\n";

    for(auto &a: node.args)
    {
        ExprGenerator eg(c, os);
        a.accept(eg);
    }

    os << "    push &\"" << sym->fullname() << type->text() << "\";\n";
    os << "    call;\n";
}
