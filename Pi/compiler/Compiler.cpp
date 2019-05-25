#include "Compiler.h"

#include "framework/Error.h"
#include "framework/Console.h"

#include "common/OpCode.h"

#include "application/Context.h"

#include "compiler/Instructions.h"

#include <pcx/lexical_cast.h>
#include <pcx/range_reverse.h>

#include <iostream>

namespace
{

void commonSizeConstruct(Context &c, Sym *sym, bool get)
{
    c.scanner.next(get);
    if(c.scanner.token().type() == Token::Type::Colon)
    {
        auto size = c.scanner.match(Token::Type::IntLiteral, true);
        sym->properties["size"] = pcx::lexical_cast<std::size_t>(size.text());

        c.scanner.next(true);
    }
}

void varConstruct(Context &c, Sym::Type type, std::vector<Sym*> *v, bool get)
{
    auto id = c.matchId(get);
    c.assertUnique(id.location(), id.text());

    auto sym = c.syms.add(new Sym(type, id.text()));
    commonSizeConstruct(c, sym, true);

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
    static int number = 1000;

    auto id = c.matchId(get);
    c.assertUnique(id.location(), id.text());

    auto sym = c.syms.add(new Sym(Sym::Type::Func, id.text()));

    c.syms.push();
    c.funcs.emplace_back(sym, c.strings.insert(id.text()));

    commonSizeConstruct(c, sym, true);

    if(c.scanner.token().type() == Token::Type::LeftBrace)
    {
        std::vector<Sym*> args;

        c.scanner.next(true);
        while(c.scanner.token().type() == Token::Type::RwArg)
        {
            varConstruct(c, Sym::Type::Arg, &args, true);
        }

        for(auto s: pcx::range_reverse(args))
        {
            s->properties["offset"] = c.func().args + (sizeof(std::size_t) * 2);
            c.func().args += s->properties["size"].to<std::size_t>();
        }

        c.func().bytes << OpCode::Op::PushR << OpCode::Reg::Bp;
        c.func().bytes << OpCode::Op::CopyRR << OpCode::Reg::Sp << OpCode::Reg::Bp;

        ByteStreamPatch lp;
        c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << lp;

        while(c.scanner.token().type() != Token::Type::RightBrace)
        {
            if(c.scanner.token().type() == Token::Type::RwVar)
            {
                varConstruct(c, Sym::Type::Local, nullptr, true);
            }
            else
            {
                Instructions::entity(c, false);
            }
        }

        c.scanner.consume(Token::Type::RightBrace, false);

        c.func().bytes << OpCode::Op::Int << number++;

        lp.patch(c.func().bytes, c.func().locals);

        c.func().bytes << OpCode::Op::AddRI << OpCode::Reg::Sp << c.func().locals;
        c.func().bytes << OpCode::Op::PopR << OpCode::Reg::Bp;
        c.func().bytes << OpCode::Op::Ret << c.func().args;
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }

    std::cout << banner(id.text());
    c.syms.print(std::cout);
    std::cout << banner("");

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
