#include "Code.h"

#include "common/OpCode.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "compiler/Instruction.h"
#include "compiler/Expr.h"

#include "visitors/AstPrinter.h"
#include "visitors/PushVisitor.h"

#include <pcx/lexical_cast.h>

namespace
{

void jmpConstruct(Context &c, bool get)
{
    auto id = c.matchId(get);
    c.pd("-jmp ", id.text());

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

void addConstruct(Context &c, bool get)
{
    auto id = c.scanner.next(get);
    c.pd("-add ", id.text());

    switch(id.type())
    {
        case Token::Type::RwInt: c.func().bytes << OpCode::Op::AddI; break;
        case Token::Type::RwSize: c.func().bytes << OpCode::Op::AddS; break;

        default: throw Error(id.location(), "invalid add type - ", id.text());
    }

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

        case Instruction::Type::Add: addConstruct(c, true); break;

        case Instruction::Type::Svc: svcConstruct(c, true); break;

        default: labelConstruct(c, false);
    }
}
