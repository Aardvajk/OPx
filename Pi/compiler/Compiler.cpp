#include "Compiler.h"

#include "framework/Error.h"
#include "framework/Console.h"

#include "common/OpCode.h"
#include "common/Interrupt.h"

#include "application/Context.h"

#include "compiler/FuncConstruct.h"

#include "components/Header.h"

#include <pcx/lexical_cast.h>
#include <pcx/range_reverse.h>

#include <iostream>

namespace
{

void commonSizeConstruct(Context &c, Sym *sym, const std::string &property, bool get)
{
    c.scanner.match(Token::Type::Colon, get);

    auto size = c.scanner.match(Token::Type::IntLiteral, true);
    sym->properties[property] = pcx::lexical_cast<std::size_t>(size.text());

    c.scanner.next(true);
}

void varConstruct(Context &c, Sym::Type type, std::vector<Sym*> *v, bool get)
{
    auto id = c.matchId(get);
    c.assertUnique(id.location(), id.text());

    auto sym = c.syms.add(new Sym(type, id.text()));
    commonSizeConstruct(c, sym, "size", true);

    if(type == Sym::Type::Arg || type == Sym::Type::Local)
    {
        if(!sym->properties["size"])
        {
            throw Error(id.location(), Sym::toString(type), " missing size - ", id.text());
        }
    }

    if(type == Sym::Type::Local)
    {
        c.func().locals += sym->properties["size"].to<std::size_t>();
        sym->properties["offset"] = c.func().locals;
    }

    if(v)
    {
        v->push_back(sym);
    }

    c.scanner.consume(Token::Type::Semicolon, false);
}

void funcConstruct(Context &c, bool get)
{
    auto id = c.matchId(get);
    c.assertUnique(id.location(), id.text());

    auto sym = c.syms.add(new Sym(Sym::Type::Func, id.text()));
    sym->properties["size"] = sizeof(std::size_t);

    c.syms.push();
    c.funcs.emplace_back(sym, c.strings.insert(id.text()));

    commonSizeConstruct(c, sym, "returnSize", true);

    if(c.scanner.token().type() == Token::Type::LeftBrace)
    {
        std::vector<Sym*> args;

        c.scanner.next(true);
        while(c.scanner.token().type() == Token::Type::RwArg)
        {
            varConstruct(c, Sym::Type::Arg, &args, true);
        }

        while(c.scanner.token().type() == Token::Type::RwVar)
        {
            varConstruct(c, Sym::Type::Local, nullptr, true);
        }

        for(auto s: pcx::range_reverse(args))
        {
            s->properties["offset"] = c.func().args + (sizeof(std::size_t) * 2);
            c.func().args += s->properties["size"].to<std::size_t>();
        }

        auto r = c.syms.add(new Sym(Sym::Type::Arg, "@ret"));
        r->properties["size"] = sym->properties["returnSize"].to<std::size_t>();
        r->properties["offset"] = c.func().args + (sizeof(std::size_t) * 2);

        c.comments("func ", sym->name, ":", sym->properties["returnSize"].to<std::size_t>());
        c.comments("{");
        c.comments(header("prologue"));

        c.func().bytes << OpCode::Op::PushR << OpCode::Reg::Bp;
        c.func().bytes << OpCode::Op::CopyRR << OpCode::Reg::Sp << OpCode::Reg::Bp;
        c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << c.func().locals;

        while(c.scanner.token().type() != Token::Type::RightBrace)
        {
            FuncConstruct::entity(c, false);
        }

        c.scanner.consume(Token::Type::RightBrace, false);

        for(auto p: c.func().jmpPatches)
        {
            auto s = c.syms.findLocal(p.first.text());
            if(!s || s->type != Sym::Type::Label)
            {
                throw Error(p.first.location(), "label expected - ", p.first.text());
            }

            p.second.patch(c.func().bytes, s->properties["position"].to<std::size_t>() - (p.second.position() + 8));
        }

        c.comments(header("----epilogue"));

        c.func().bytes << OpCode::Op::AddRI << OpCode::Reg::Sp << c.func().locals;
        c.func().bytes << OpCode::Op::PopR << OpCode::Reg::Bp;
        c.func().bytes << OpCode::Op::Ret << c.func().args;

        c.comments("}\n");
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }

    std::cout << banner(id.text());
    c.syms.print(std::cout);

    c.syms.pop();
}

void construct(Context &c, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwVar: varConstruct(c, Sym::Type::Global, nullptr, true); break;
        case Token::Type::RwFunc: funcConstruct(c, true); break;

        default: throw Error(tok.location(), "construct expected - ", tok.text());
    }
}

}

void compile(Context &c)
{
    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::Eof)
    {
        construct(c, false);
    }
}
