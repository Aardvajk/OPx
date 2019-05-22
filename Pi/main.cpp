#include "error/Error.h"

#include "application/Context.h"

#include "compiler/Compiler.h"

#include <pcx/str.h>

std::string banner(const std::string &title);

#include "opcode/OpCode.h"
#include <fstream>

int main(int argc, char *argv[])
{
    Context c;

    try
    {
        c.open("C:/Projects/Px/Px/Pi/script.txt");

        std::cout << banner("");

        compile(c);

        std::cout << banner("");

        c.syms.print(std::cout);

        std::cout << banner("");

        for(auto &f: c.funcs)
        {
            std::cout << "func " << f.sym->name << "\n";
        }

        std::cout << banner("");

        if(true)
        {
            std::ofstream os("C:/Users/aardv/Desktop/prog.px", std::ios::binary);
            ByteStream bs(os);

            ByteStreamPatch p;
            ByteStreamPatch o;

            using namespace OpCode;

            // space for return
            bs << Type::SubRI << Reg::Sp << std::size_t(4);

            // push pc
            bs << Type::CopyRR << Reg::Pc << Reg::Dx;

            auto pos = bs.position();
            bs << Type::AddRI << Reg::Dx << o;
            bs << Type::PushR << Reg::Dx;

            // push param
            bs << Type::SubRI << Reg::Sp << std::size_t(4);
            bs << Type::CopyAI << Reg::Sp << std::size_t(4) << 321;

            // push func address
            bs << Type::SubRI << Reg::Sp << std::size_t(8);
            bs << Type::CopyAI << Reg::Sp << std::size_t(8) << p;

            // pop address into pc
            bs << Type::PopR << Reg::Pc;

            o.patch(bs, bs.position() - pos);

            // return point
            bs << Type::Int << 3;

            // pop int
            bs << Type::AddRI << Reg::Sp << std::size_t(4);

            bs << Type::Int << 1010;
            bs << Type::End;

            p.patch(bs, bs.position());

            // function
            bs << Type::Int << 3;

            // get address of int in dx
            bs << Type::CopyRR << Reg::Sp << Reg::Dx;
            bs << Type::AddRI << Reg::Dx << std::size_t(12);

            // copy in to stack
            bs << Type::CopyAI << Reg::Dx << std::size_t(4) << 123;

            // destroy param
            bs << Type::AddRI << Reg::Sp << std::size_t(4);

            // get return address from stack
            bs << Type::PopR << Reg::Pc;
        }

        auto r = std::system("C:/Projects/Px/Px/build-Pv/release/pv C:/Users/aardv/Desktop/prog.px");

        std::cout << banner("");
    }

    catch(const Error &error)
    {
        std::cerr << "error";
        if(auto n = error.location())
        {
            std::cerr << " " << c.sources.path(n.id()) << " " << n.line() << "," << n.column();
        }

        std::cerr << ": " << error.what() << "\n";
    }
}

std::string banner(const std::string &title)
{
    if(title.empty())
    {
        return std::string(120, '=');
    }

    return pcx::str("== ", title, " ", std::string(120 - (title.length() + 4), '='));
}

