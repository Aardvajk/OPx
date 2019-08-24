#include "application/Context.h"

#include "framework/Error.h"
#include "framework/Console.h"

#include "compiler/Compiler.h"

#include <pcx/args.h>

#include <fstream>

int main(int argc, char *argv[])
{
    Context c;

    try
    {
        std::vector<std::string> files;
        pcx::args args(argc, argv, files);

        if(files.size() < 1)
        {
            throw Error("no source specified");
        }

        if(files.size() < 2)
        {
            throw Error("no output specified");
        }

        c.open(files[0]);
        compile(c);

        std::string output = files[1];

        if(!args.contains("q"))
        {
            std::cout << banner("pi ", output);
        }

        if(true)
        {
            std::ofstream os(output, std::ios::binary);
            ByteStream bs(os);

            bs << c.strings.size();
            for(auto &s: c.strings)
            {
                bs << s;
            }

            bs << c.globs.size() + c.funcs.size();
            for(auto &g: c.globs)
            {
                bs << 'V' << g.id;

                bs << std::size_t(0);

                auto bytes = g.bytes.data();

                bs << bytes.size();
                bs.write(bytes.data(), bytes.size());
            }

            for(auto &f: c.funcs)
            {
                ByteStreamPatch p;

                bs << 'F' << f.id;

                bs << f.links.size();
                for(auto i: f.links)
                {
                    bs << i.address << i.id;
                }

                auto bytes = f.bytes.data();

                bs << bytes.size();
                bs.write(bytes.data(), bytes.size());
            }
        }

        if(!args.contains("q"))
        {
            std::cout << "created " << output << "\n";
        }

        if(true)
        {
            std::ofstream os(output + ".pmap");
            if(!os.is_open())
            {
                throw Error("unable to create - ", output, ".pmap");
            }

            c.vd.write(os);
            c.pd.write(os);

            if(!args.contains("q"))
            {
                std::cout << "created " << output << ".pmap" << "\n";
            }
        }
    }

    catch(const Error &error)
    {
        std::cerr << "pi error";
        if(auto n = error.location())
        {
            std::cerr << " " << c.sources.path(n.id()) << " " << n.line() << "," << n.column();
        }

        std::cerr << ": " << error.what() << "\n";
        return -1;
    }
}
