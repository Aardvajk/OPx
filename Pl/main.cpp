#include "framework/Error.h"
#include "framework/Console.h"

#include "application/Context.h"
#include "application/Prologue.h"
#include "application/Generator.h"
#include "application/Composor.h"
#include "application/Linker.h"
#include "application/GlobalFuncs.h"

#include <pcx/args.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

void generateRefs(Context &c, Object::Entity *e)
{
    for(auto n: e->links)
    {
        auto r = c.units[e->unit].strings[n.id];
        c.refs.insert(r);

        auto re = c.find(r);
        if(re)
        {
            generateRefs(c, re);
        }
    }
}

void updateDebugMap(Context &c, DebugMap &dm)
{
    DebugMap rm;
    std::unordered_set<std::string> done;

    for(std::size_t i = 0; i < dm.size(); ++i)
    {
        if(dm[i].name.empty() || c.refs.find(dm[i].name) != c.refs.end())
        {
            if(done.find(dm[i].name) == done.end())
            {
                rm.push_back(dm[i]);
                done.insert(dm[i].name);
            }
        }
    }

    dm = rm;
}

int main(int argc, char *argv[])
{
    try
    {
        std::vector<std::string> files;
        Context c(argc, argv, files);

        if(files.size() < 1)
        {
            throw Error("no output specified");
        }

        std::string output = files[0];

        if(files.size() < 2)
        {
            throw Error("no sources specified");
        }

        std::vector<std::string> paths;
        for(std::size_t i = 1; i < files.size(); ++i)
        {
            paths.push_back(files[i]);
        }

        auto mp = Prologue::generate(c);

        if(!c.args.contains("q"))
        {
            std::cout << banner("linking");
        }

        for(auto &p: paths)
        {
            if(!c.args.contains("q"))
            {
                std::cout << p << "\n";
            }

            Generator::generate(c, p);
        }

        auto me = c.find("main():std.null");
        if(!me)
        {
            throw Error("main():std.null not found");
        }

        if(c.args.contains("trim"))
        {
            c.refs.insert("main():std.null");
            generateRefs(c, me);

            for(auto n: c.globalInits)
            {
                c.refs.insert(n);
                generateRefs(c, c.find(n));
            }

            for(auto n: c.globalDestroys)
            {
                c.refs.insert(n);
                generateRefs(c, c.find(n));
            }
        }

        Composor::compose(c);

        Linker::link(c);

        auto gi = GlobalFuncs::generateGlobalInit(c, me->offset);
        mp.patch(c.ds, c.ds.position() + gi);

        if(true)
        {
            std::ofstream os(output, std::ios::binary);
            if(!os.is_open())
            {
                throw Error("unable to create - ", output);
            }

            auto db = c.ds.data();
            auto pb = c.ps.data();

            os.write(db.data(), std::streamoff(db.size()));
            os.write(pb.data(), std::streamoff(pb.size()));
        }

        if(true)
        {
            if(c.args.contains("trim"))
            {
                updateDebugMap(c, c.vd);
                updateDebugMap(c, c.pd);
            }

            auto s = pcx::str(output, ".pmap");

            std::ofstream os(s);
            if(!os.is_open())
            {
                throw Error("unable to create - ", s);
            }

            c.vd.write(os);
            c.pd.write(os);
        }
    }

    catch(const Error &error)
    {
        std::cerr << "pl error: " << error.what() << "\n";
        return -1;
    }
}
