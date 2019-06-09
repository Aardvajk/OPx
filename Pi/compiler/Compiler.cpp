#include "Compiler.h"

#include "framework/Error.h"
#include "framework/Console.h"

#include "scanner/Lexer.h"

#include "common/OpCode.h"

#include "application/Context.h"

#include "compiler/Code.h"

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

template<typename T> void writeBytes(std::vector<char> &v, const T &value)
{
    const char *s = reinterpret_cast<const char*>(&value);

    for(std::size_t i = 0; i < sizeof(T); ++i)
    {
        v.push_back(s[i]);
    }
}

void byteListConstruct(Context &c, std::vector<char> &v, bool get)
{
    auto tok = c.scanner.next(get);

    if(tok.type() == Token::Type::StringLiteral)
    {
        auto s = Lexer::decodeString(tok.text());
        for(std::size_t i = 0; i < s.length(); ++i)
        {
            v.push_back(s[i]);
        }
    }
    else if(tok.text() == "int" || tok.text() == "long")
    {
        c.scanner.match(Token::Type::LeftParen, true);
        auto value = c.scanner.match(Token::Type::IntLiteral, true);

        if(tok.text() == "int")
        {
            writeBytes(v, pcx::lexical_cast<int>(value.text()));
        }
        else if(tok.text() == "long")
        {
            writeBytes(v, pcx::lexical_cast<std::size_t>(value.text()));
        }

        c.scanner.match(Token::Type::RightParen, true);
    }
    else
    {
        v.push_back(char(pcx::lexical_cast<int>(tok.text())));
    }

    tok = c.scanner.next(true);
    if(tok.type() == Token::Type::Comma)
    {
        byteListConstruct(c, v, true);
    }
}

void varConstruct(Context &c, Sym::Type type, std::vector<Sym*> *v, bool get)
{
    auto id = c.matchId(get);
    c.assertUnique(id.location(), id.text());

    auto sym = c.syms.add(new Sym(type, id.text()));

    auto next = c.scanner.next(true);
    if(next.type() == Token::Type::Colon)
    {
        commonSizeConstruct(c, sym, "size", false);
    }

    if(type == Sym::Type::Global)
    {
        std::vector<char> v;
        if(c.scanner.token().type() == Token::Type::Assign)
        {
            byteListConstruct(c, v, true);

            if(!sym->properties["size"])
            {
                sym->properties["size"] = std::size_t(v.size());
            }

            if(v.size() > sym->properties["size"].to<std::size_t>())
            {
                throw Error(c.scanner.token().location(), "too many bytes - ", id.text());
            }
        }
        else
        {
            v.push_back(0);
        }

        c.globs.emplace_back(sym, c.strings.insert(id.text()));

        auto sz = sym->properties["size"].to<std::size_t>();
        for(std::size_t i = 0; i < sz; ++i)
        {
            c.globs.back().bytes << (i < v.size() ? v[i] : v.back());
        }

        c.vd.begin('V', sym->name, sz, { });
        c.vd("var ", sym->name, ":", sz);

    }
    else if(type == Sym::Type::Local)
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

    Sym *sym = c.syms.find(id.text());
    if(sym)
    {
        if(sym->type != Sym::Type::Func)
        {
            throw Error(id.location(), "function expected - ", id.text());
        }
    }
    else
    {
        sym = c.syms.add(new Sym(Sym::Type::Func, id.text()));
        sym->properties["size"] = sizeof(std::size_t);
    }

    auto rs = sym->properties["returnSize"];

    commonSizeConstruct(c, sym, "returnSize", true);

    if(rs && rs.to<std::size_t>() != sym->properties["returnSize"].to<std::size_t>())
    {
        throw Error(id.location(), "return size different - ", id.text());
    }

    if(c.scanner.token().type() == Token::Type::LeftBrace)
    {
        if(sym->properties["defined"].value<bool>())
        {
            throw Error(id.location(), "function already defined - ", id.text());
        }

        sym->properties["defined"] = true;

        c.syms.push();
        c.funcs.emplace_back(sym, c.strings.insert(id.text()));

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

        c.pd.begin('F', sym->name, 0, pcx::make_callback(&c, &Context::funcPosition));

        c.pd("func ", sym->name, ":", sym->properties["returnSize"].to<std::size_t>());
        c.pd("{");

        c.pd("-function prologue");
        c.func().bytes << OpCode::Op::PushR << OpCode::Reg::Bp;
        c.func().bytes << OpCode::Op::CopyRR << OpCode::Reg::Sp << OpCode::Reg::Bp;

        c.pd("-allocate locals");
        c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << c.func().locals;

        while(c.scanner.token().type() != Token::Type::RightBrace)
        {
            Code::construct(c, false);
        }

        c.scanner.consume(Token::Type::RightBrace, false);

        for(auto &j: c.func().jumps)
        {
            auto s = c.syms.findLocal(j.first.text());
            if(!s || s->type != Sym::Type::Label)
            {
                throw Error(j.first.location(), "label expected - ", j.first.text());
            }

            auto pos = s->properties["position"].to<std::size_t>();
            j.second.patch(c.func().bytes, pos - (j.second.position() + sizeof(std::size_t)));
        }

        c.pd("-free locals");
        c.func().bytes << OpCode::Op::AddRI << OpCode::Reg::Sp << c.func().locals;

        c.pd("-function epilogue");
        c.func().bytes << OpCode::Op::PopR << OpCode::Reg::Bp;
        c.func().bytes << OpCode::Op::Ret << c.func().args;

        c.pd("-end func ", sym->name);
        c.pd("}");

        c.pd.back().size = c.func().bytes.position();

        c.syms.pop();
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }
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
