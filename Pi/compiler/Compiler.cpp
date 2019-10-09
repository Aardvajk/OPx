#include "Compiler.h"

#include "framework/Error.h"
#include "framework/Console.h"

#include "scanner/Lexer.h"

#include "common/OpCode.h"
#include "common/Object.h"

#include "application/Context.h"

#include "compiler/Code.h"
#include "compiler/Expr.h"

#include "visitors/ByteWriter.h"
#include "visitors/AstPrinter.h"

#include <pcx/lexical_cast.h>
#include <pcx/range_reverse.h>

#include <iostream>

namespace
{

void commonSizeConstruct(Context &c, Location location, Sym *sym, const std::string &property, bool get)
{
    c.scanner.match(Token::Type::Colon, get);

    auto tok = c.scanner.match(Token::Type::IntLiteral, true);
    auto size = pcx::lexical_cast<std::size_t>(tok.text());

    if(auto ex = sym->properties[property])
    {
        if(ex.to<std::size_t>() != size)
        {
            throw Error(location, "mismatched ", property, " - ", sym->name);
        }
    }

    sym->properties[property] = size;

    c.scanner.next(true);
}

void byteListConstruct(Context &c, std::vector<char> &v, bool get)
{
    auto expr = Expr::get(c, get);

    ByteWriter br(v);
    expr->accept(br);

    if(!br.okay())
    {
        throw Error(expr->location(), "constant expected - ", astReconstruct(expr.get()));
    }

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        byteListConstruct(c, v, true);
    }
}

void propertyConstruct(Context &c, std::vector<std::string> &container, bool get)
{
    auto tok = c.matchId(get);
    container.push_back(tok.text());

    tok = c.scanner.next(true);
    if(tok.type() == Token::Type::Comma)
    {
        propertyConstruct(c, container, true);
    }
}

std::vector<std::string> properties(Context &c, bool get)
{
    std::vector<std::string> props;

    auto tok = c.scanner.next(get);
    if(tok.type() == Token::Type::LeftSub)
    {
        tok = c.scanner.next(true);
        if(tok.type() != Token::Type::RightSub)
        {
            propertyConstruct(c, props, false);
        }

        c.scanner.consume(Token::Type::RightSub, false);
    }

    return props;
}

void processProperties(Context &c, Sym *sym, const std::vector<std::string> &props)
{
    Object::Entity::Flags flags;

    for(auto &s: props)
    {
        if(s == "autogen")
        {
            flags |= Object::Entity::Flag::AutoGen;
        }
        else if(s == "globalinit")
        {
            flags |= Object::Entity::Flag::GlobalInit;
        }
        else if(s == "globaldestroy")
        {
            flags |= Object::Entity::Flag::GlobalDestroy;
        }
    }

    sym->properties["flags"] = flags;
}

void varConstruct(Context &c, Sym::Type type, std::vector<Sym*> *v, bool get)
{
    bool external = false;

    auto tok = c.scanner.next(get);
    if(tok.type() == Token::Type::RwExternal)
    {
        external = true;
        c.scanner.next(true);
    }

    auto id = c.matchId(false);

    Sym *sym = c.syms.find(id.text());
    if(sym)
    {
        if(sym->type != Sym::Type::Global)
        {
            throw Error(id.location(), "global var expected - ", id.text());
        }

        if(!sym->properties["external"].value<bool>() && !external)
        {
            throw Error(id.location(), "already defined - ", id.text());
        }
    }
    else
    {
        sym = c.syms.add(new Sym(type, id.text()));

        sym->properties["external"] = external;
        sym->properties["flags"] = Object::Entity::Flags();
    }

    auto next = c.scanner.next(true);
    if(next.type() == Token::Type::Colon)
    {
        commonSizeConstruct(c, next.location(), sym, "size", false);
    }

    if(type == Sym::Type::Global)
    {
        if(!external)
        {
            std::vector<char> v;
            if(c.scanner.token().type() == Token::Type::Assign)
            {
                byteListConstruct(c, v, true);

                if(!sym->properties["size"])
                {
                    sym->properties["size"] = std::size_t(v.size());
                }
                else if(sym->properties["size"].to<std::size_t>() != std::size_t(v.size()))
                {
                    throw Error(next.location(), "mismatched size - ", sym->name);
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
    auto props = properties(c, get);
    auto id = c.matchId(false);

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

    processProperties(c, sym, props);

    auto rs = sym->properties["returnSize"];

    commonSizeConstruct(c, id.location(), sym, "returnSize", true);

    if(rs && rs.to<std::size_t>() != sym->properties["returnSize"].to<std::size_t>())
    {
        throw Error(id.location(), "return size different - ", id.text());
    }

    if(c.scanner.token().type() == Token::Type::LeftBrace)
    {
        if(sym->properties["defined"].value<bool>())
        {
            throw Error(id.location(), "already defined - ", id.text());
        }

        sym->properties["defined"] = true;

        c.scanner.match(Token::Type::LeftBrace, false);

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
