#include "FuncConstruct.h"

#include "framework/Error.h"

#include "common/OpCode.h"

#include "application/Context.h"

#include "compiler/Instruction.h"

#include <pcx/lexical_cast.h>

namespace
{

void generateAddress(Context &c, OpCode::Reg reg, Sym *sym)
{
    if(sym->type == Sym::Type::Arg || sym->type == Sym::Type::Local)
    {
        using namespace OpCode;

        c.func().bytes << Op::CopyRR << Reg::Bp << reg;
        c.func().bytes << (sym->type == Sym::Type::Arg ? Op::AddRI : Op::SubRI) << reg << sym->properties["offset"].to<std::size_t>();
    }
}

void pushInstruction(Context &c, bool get)
{
    auto tok = c.scanner.next(get);

    if(tok.type() == Token::Type::IntLiteral)
    {
        c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << sizeof(int);
        c.func().bytes << OpCode::Op::CopyAI << OpCode::Reg::Sp << sizeof(int) << pcx::lexical_cast<int>(tok.text());
    }
    else
    {
        c.matchId(false);
        auto sym = c.syms.find(tok.text());
        if(!sym)
        {
            throw Error(tok.location(), "not found - ", tok.text());
        }

        if(sym->type == Sym::Type::Arg || sym->type == Sym::Type::Local)
        {
            generateAddress(c, OpCode::Reg::Dx, sym);

            auto sz = sym->properties["size"].to<std::size_t>();

            c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << sz;
            c.func().bytes << OpCode::Op::CopyAA << OpCode::Reg::Dx << OpCode::Reg::Sp << sz;
        }
        else if(sym->type == Sym::Type::Func)
        {
            auto id = c.strings.insert(tok.text());

            c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << sizeof(std::size_t);
            c.func().bytes << OpCode::Op::CopyAI << OpCode::Reg::Sp << sizeof(std::size_t) << std::size_t(0);

            c.func().links.push_back(Object::Link(id, c.func().bytes.position() - sizeof(std::size_t)));
        }
    }

    c.scanner.consume(Token::Type::Semicolon, true);
}

void popInstruction(Context &c, bool get)
{
    auto id = c.scanner.match(Token::Type::IntLiteral, get);

    c.func().bytes << OpCode::Op::AddRI << OpCode::Reg::Sp << pcx::lexical_cast<std::size_t>(id.text());

    c.scanner.consume(Token::Type::Semicolon, true);
}

void storeInstruction(Context &c, bool get)
{
    auto tok = c.matchId(get);

    auto sym = c.syms.find(tok.text());

    if(sym && (sym->type == Sym::Type::Arg || sym->type == Sym::Type::Local))
    {
        generateAddress(c, OpCode::Reg::Dx, sym);

        auto sz = sym->properties["size"].to<std::size_t>();
        c.func().bytes << OpCode::Op::CopyAA << OpCode::Reg::Sp << OpCode::Reg::Dx << sz;
    }
    else
    {
    }

    c.scanner.consume(Token::Type::Semicolon, true);
}

void allocSInstruction(Context &c, bool get)
{
    auto id = c.scanner.match(Token::Type::IntLiteral, get);

    c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << pcx::lexical_cast<std::size_t>(id.text());

    c.scanner.consume(Token::Type::Semicolon, true);
}

void jmpInstruction(Context &c, bool get)
{
    auto label = c.matchId(get);

    if(auto s = c.syms.findLocal(label.text()))
    {
        if(s->type != Sym::Type::Label)
        {
            throw Error(label.location(), "label expected - ", label.text());
        }

        ByteStreamPatch p;
        c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Pc << p;

        p.patch(c.func().bytes, c.func().bytes.position() - s->properties["position"].to<std::size_t>());
    }
    else
    {
        ByteStreamPatch p;
        c.func().bytes << OpCode::Op::AddRI << OpCode::Reg::Pc << p;

        c.func().jmpPatches.push_back(std::make_pair(label, p));
    }

    c.scanner.consume(Token::Type::Semicolon, true);
}

void callInstruction(Context &c, bool get)
{
    c.func().bytes << OpCode::Op::PopR << OpCode::Reg::Dx;
    c.func().bytes << OpCode::Op::Call << OpCode::Reg::Dx;

    c.scanner.consume(Token::Type::Semicolon, get);
}

void intInstruction(Context &c, bool get)
{
    auto id = c.scanner.match(Token::Type::IntLiteral, get);

    c.func().bytes << OpCode::Op::Int << pcx::lexical_cast<int>(id.text());

    c.scanner.consume(Token::Type::Semicolon, true);
}

}

void FuncConstruct::entity(Context &c, bool get)
{
    auto tok = c.scanner.next(get);

    auto next = c.scanner.next(true);
    if(next.type() == Token::Type::Colon)
    {
        c.assertUnique(tok.location(), tok.text());

        auto sym = c.syms.add(new Sym(Sym::Type::Label, tok.text()));
        sym->properties["position"] = c.func().bytes.position();

        c.scanner.next(true);
        return;
    }

    auto code = Instruction::fromString(tok.text());
    switch(code)
    {
        case Instruction::Code::Push: pushInstruction(c, false); break;
        case Instruction::Code::Pop: popInstruction(c, false); break;
        case Instruction::Code::Store: storeInstruction(c, false); break;
        case Instruction::Code::AllocS: allocSInstruction(c, false); break;

        case Instruction::Code::Jmp: jmpInstruction(c, false); break;
        case Instruction::Code::Call: callInstruction(c, false); break;

        case Instruction::Code::Int: intInstruction(c, false); break;

        default: throw Error(tok.location(), "instruction expected - ", tok.text());
    }
}
