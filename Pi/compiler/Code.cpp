#include "Code.h"

#include "common/OpCode.h"
#include "common/Primitive.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "compiler/Instruction.h"
#include "compiler/Expr.h"

#include "visitors/AstPrinter.h"
#include "visitors/PushVisitor.h"

#include <pcx/lexical_cast.h>

namespace
{

Primitive::Type primitive(Token::Type type)
{
    switch(type)
    {
        case Token::Type::RwChar: return Primitive::Type::Char;
        case Token::Type::RwInt: return Primitive::Type::Int;
        case Token::Type::RwSize: return Primitive::Type::Size;

        default: return Primitive::Type::Invalid;
    }
}

Primitive::Type primitiveFromToken(const Token &token)
{
    auto r = primitive(token.type());
    if(r == Primitive::Type::Invalid)
    {
        throw Error(token.location(), "primitive expected - ", token.text());
    }

    return r;
}

void jmpConstruct(Context &c, bool get)
{
    bool ifz = false;

    auto tok = c.scanner.next(get);
    if(tok.type() == Token::Type::Id && tok.text() == "ifz")
    {
        ifz = true;
        c.scanner.next(true);
    }

    auto id = c.matchId(false);
    c.pd("-jmp ", (ifz ? "ifz " : ""), id.text());

    if(ifz)
    {
        c.func().bytes << OpCode::Op::JmpZ << std::size_t(10);
    }

    if(auto s = c.syms.findLocal(id.text()))
    {
        if(s->type != Sym::Type::Label)
        {
            throw Error(id.location(), "label expected - ", id.text());
        }

        ByteStreamPatch p;
        c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Pc << p;

        auto pos = s->properties["position"].to<std::size_t>();
        p.patch(c.func().bytes, c.func().bytes.position() - pos);
    }
    else
    {
        ByteStreamPatch p;
        c.func().bytes << OpCode::Op::AddRI << OpCode::Reg::Pc << p;

        c.func().jumps.emplace_back(id, p);
    }

    c.scanner.consume(Token::Type::Semicolon, true);
}

void callConstruct(Context &c, bool get)
{
    c.pd("-call");

    c.func().bytes << OpCode::Op::PopR << OpCode::Reg::Dx;
    c.func().bytes << OpCode::Op::Call << OpCode::Reg::Dx;

    c.scanner.consume(Token::Type::Semicolon, get);
}

void pushConstruct(Context &c, bool get)
{
    auto expr = Expr::get(c, true);

    c.pd("-push ", astReconstruct(expr.get()));

    PushVisitor pv(c);
    expr->accept(pv);

    if(!pv.okay())
    {
        throw Error(expr->location(), "push syntax invalid - ", astReconstruct(expr.get()));
    }

    c.scanner.consume(Token::Type::Semicolon, false);
}

void popConstruct(Context &c, bool get)
{
    auto id = c.scanner.match(Token::Type::IntLiteral, get);
    c.pd("-pop ", id.text());

    c.func().bytes << OpCode::Op::AddRI << OpCode::Reg::Sp << pcx::lexical_cast<std::size_t>(id.text());

    c.scanner.consume(Token::Type::Semicolon, true);
}

void allocSConstruct(Context &c, bool get)
{
    auto id = c.scanner.match(Token::Type::IntLiteral, get);
    c.pd("-allocs ", id.text());

    c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << pcx::lexical_cast<std::size_t>(id.text());

    c.scanner.consume(Token::Type::Semicolon, true);
}

void loadConstruct(Context &c, bool get)
{
    auto id = c.scanner.match(Token::Type::IntLiteral, get);
    c.pd("-load ", id.text());

    auto sz = pcx::lexical_cast<std::size_t>(id.text());

    c.func().bytes << OpCode::Op::PopR << OpCode::Reg::Dx;
    c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << sz;
    c.func().bytes << OpCode::Op::CopyAA << OpCode::Reg::Dx << OpCode::Reg::Sp << sz;

    c.scanner.consume(Token::Type::Semicolon, true);
}

void storeConstruct(Context &c, bool get)
{
    auto id = c.scanner.match(Token::Type::IntLiteral, get);
    c.pd("-store ", id.text());

    c.func().bytes << OpCode::Op::PopR << OpCode::Reg::Dx;
    c.func().bytes << OpCode::Op::CopyAA << OpCode::Reg::Sp << OpCode::Reg::Dx << pcx::lexical_cast<std::size_t>(id.text());

    c.scanner.consume(Token::Type::Semicolon, true);
}

void opConstruct(Context &c, std::string name, OpCode::Op op, bool get)
{
    auto id = c.scanner.next(get);
    c.pd("-", name, " ", id.text());

    c.func().bytes << op << primitiveFromToken(id);

    c.scanner.consume(Token::Type::Semicolon, true);
}

void convertConstruct(Context &c, bool get)
{
    auto s = c.scanner.next(get);
    auto d = c.scanner.next(get);

    c.pd("-convert ", s.text(), " ", d.text());

    c.func().bytes << OpCode::Op::Conv << primitiveFromToken(s) << primitiveFromToken(d);

    c.scanner.consume(Token::Type::Semicolon, true);
}

void allocConstruct(Context &c, bool get)
{
    c.pd("-alloc");

    c.func().bytes << OpCode::Op::Alloc;

    c.scanner.consume(Token::Type::Semicolon, true);
}

void freeConstruct(Context &c, bool get)
{
    c.pd("-free");

    c.func().bytes << OpCode::Op::Free;

    c.scanner.consume(Token::Type::Semicolon, true);
}

void svcConstruct(Context &c, bool get)
{
    auto id = c.scanner.match(Token::Type::IntLiteral, get);
    c.pd("-svc ", id.text());

    c.func().bytes << OpCode::Op::Service << pcx::lexical_cast<int>(id.text());

    c.scanner.consume(Token::Type::Semicolon, true);
}

void labelConstruct(Context &c, bool get)
{
    auto id = c.matchId(get);
    c.pd("-label ", id.text());

    c.assertUnique(id.location(), id.text());

    auto sym = c.syms.add(new Sym(Sym::Type::Label, id.text()));
    sym->properties["position"] = c.func().bytes.position();

    c.scanner.consume(Token::Type::Colon, true);
}

}

void Code::construct(Context &c, bool get)
{
    auto tok = c.scanner.next(get);
    auto ins = Instruction::fromString(tok.text());

    switch(ins)
    {
        case Instruction::Type::Jmp: jmpConstruct(c, true); break;
        case Instruction::Type::Call: callConstruct(c, true); break;

        case Instruction::Type::Push: pushConstruct(c, true); break;
        case Instruction::Type::Pop: popConstruct(c, true); break;
        case Instruction::Type::AllocS: allocSConstruct(c, true); break;

        case Instruction::Type::Load: loadConstruct(c, true); break;
        case Instruction::Type::Store: storeConstruct(c, true); break;

        case Instruction::Type::Add: opConstruct(c, "add", OpCode::Op::Add, true); break;
        case Instruction::Type::Sub: opConstruct(c, "sub", OpCode::Op::Sub, true); break;
        case Instruction::Type::Mul: opConstruct(c, "mul", OpCode::Op::Mul, true); break;
        case Instruction::Type::Div: opConstruct(c, "div", OpCode::Op::Div, true); break;
        case Instruction::Type::Mod: opConstruct(c, "mod", OpCode::Op::Mod, true); break;

        case Instruction::Type::Not: opConstruct(c, "not", OpCode::Op::Not, true); break;
        case Instruction::Type::Neg: opConstruct(c, "neg", OpCode::Op::Neg, true); break;

        case Instruction::Type::Lt: opConstruct(c, "lt", OpCode::Op::Lt, true); break;
        case Instruction::Type::LtEq: opConstruct(c, "lteq", OpCode::Op::LtEq, true); break;

        case Instruction::Type::Alloc: allocConstruct(c, true); break;
        case Instruction::Type::Free: freeConstruct(c, true); break;

        case Instruction::Type::Convert: convertConstruct(c, true); break;

        case Instruction::Type::Svc: svcConstruct(c, true); break;

        default: labelConstruct(c, false);
    }
}
