#include "Generator.h"

#include "application/Context.h"
#include "application/Pragmas.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"

#include "syms/Sym.h"

#include "types/Type.h"
#include "types/TypeLookup.h"

#include "generator/LocalsGenerator.h"
#include "generator/FuncGenerator.h"
#include "generator/ByteListGenerator.h"

#include "visitors/TypeVisitor.h"

#include <pcx/range_reverse.h>

Generator::Generator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void Generator::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void Generator::visit(NamespaceNode &node)
{
    auto sg = c.tree.open(node.property<Sym*>("sym"));
    node.body->accept(*this);
}

void Generator::visit(FuncNode &node)
{
    if(node.body && (!node.autoGen || !c.option("debug", "suppress_autogens")))
    {
        auto sym = node.property<Sym*>("sym");
        auto type = sym->property<Type*>("type");

        os << "func" << (node.autoGen ? "[autogen]" : "");
        os << " \"" << sym->fullname() << type->text() << "\":" << Type::assertSize(node.location(), type->returnType) << "\n";
        os << "{\n";

        for(auto &a: node.args)
        {
            auto s = a->property<Sym*>("sym");
            os << "    arg \"" << s->fullname() << "\":" << Type::assertSize(a->location(), s->property<Type*>("type")) << ";\n";
        }

        auto sg = c.tree.open(sym);

        Visitor::visit<LocalsGenerator>(node.body.get(), c, os);

        auto info = sym->property<FuncInfo*>("info");
        for(auto &t: info->temps)
        {
            os << "    var \"" << t.first << "\":" << Type::assertSize(node.location(), t.second) << ";\n";
        }

        if(!c.option("O", "elide_unneeded_complex_returns") || sym->findProperty("complexReturns").value<bool>())
        {
            os << "    var \"@rf\":1;\n";
            os << "    clrf \"@rf\";\n";
        }

        Visitor::visit<FuncGenerator>(node.body.get(), c, os);

        if(sym->findProperty("endFunctionRef").value<bool>() || !c.option("O", "elide_unused_targets"))
        {
            os << "\"#end_function\":\n";
        }

        if(sym->name() == "delete")
        {
            auto cs = sym->parent()->children();

            for(auto s: pcx::range_reverse(cs))
            {
                if(s->type() == Sym::Type::Var)
                {
                    auto t = s->property<Type*>("type");
                    if(!t->primitive())
                    {
                        auto dm = TypeLookup::assertDeleteMethod(c, node.location(), t);

                        os << "    push \"" << sym->fullname() << ".this\";\n";

                        auto o = s->property<std::size_t>("offset");

                        if(!c.option("O", "elide_no_effect_ops") || o)
                        {
                            os << "    push size(" << o << ");\n";
                            os << "    add size;\n";
                        }

                        os << "    push &\"" << dm->funcname()<< "\";\n";
                        os << "    call;\n";
                    }
                }
            }
        }

        for(auto a: pcx::range_reverse(node.args))
        {
            auto s = a->property<Sym*>("sym");
            auto t = s->property<Type*>("type");

            if(!t->primitive())
            {
                auto dm = TypeLookup::assertDeleteMethod(c, a->location(), t);

                os << "    push &\"" << s->fullname() << "\";\n";
                os << "    push &\"" << dm->funcname() << "\";\n";
                os << "    call;\n";
            }
        }

        os << "}\n";
    }
}

void Generator::visit(ClassNode &node)
{
    if(node.body)
    {
        auto sg = c.tree.open(node.property<Sym*>("sym"));
        node.body->accept(*this);
    }
}

void Generator::visit(VarNode &node)
{
    auto sym = node.property<Sym*>("sym");

    if(!sym->findProperty("member").value<bool>())
    {
        os << "var \"" << sym->fullname() << "\":" << Type::assertSize(node.location(), sym->property<Type*>("type"));

        if(node.value)
        {
            if(!TypeVisitor::assertType(c, node.value.get())->primitive())
            {
                throw Error(node.location(), "non-primitive global initialisers not supported - ", node.value->description());
            }

            os << " = ";
            Visitor::query<ByteListGenerator, bool>(node.value.get(), c, os);
        }

        os << ";\n";
    }
}

void Generator::visit(PragmaNode &node)
{
    Pragmas::execute(c, node);
}
