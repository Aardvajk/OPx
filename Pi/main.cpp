#include "application/Context.h"

#include "framework/Error.h"
#include "framework/Console.h"

#include "compiler/Compiler.h"

#include <fstream>

int main(int argc, char *argv[])
{
    Context c;

    try
    {
        if(argc < 2)
        {
            throw Error("no source specified");
        }

        c.open(argv[1]);
        compile(c);

        if(true)
        {
            std::ofstream os("C:/Projects/Px/Px/script.po", std::ios::binary);
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

        if(true)
        {
            std::ofstream os("C:/Projects/Px/Px/script.po.pmap");
            if(!os.is_open())
            {
                throw Error("unable to create - script.po.pmap");
            }

            c.vd.write(os);
            c.pd.write(os);
        }

        checked_system("C:/Projects/Px/Px/build-Pd/release/pd C:/Projects/Px/Px/script.po C:/Projects/Px/Px/script.po.pmap");
        checked_system("C:/Projects/Px/Px/build-Pl/release/pl C:/Projects/Px/Px/script.po");
    }

    catch(const Error &error)
    {
        std::cerr << "pi error";
        if(auto n = error.location())
        {
            std::cerr << " " << c.sources.path(n.id()) << " " << n.line() << "," << n.column();
        }

        std::cerr << ": " << error.what() << "\n";
    }
}
