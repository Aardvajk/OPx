#include "Comments.h"

#include <pcx/lexical_cast.h>

#include <fstream>
#include <cctype>

const Comments::Bare_T Bare;

Comments::Comments() : ff(false)
{
}

Comments::Comments(pcx::callback<std::size_t> cb) : ff(false), cb(cb)
{
}

Comments::Comments(const std::string &path) : ff(false)
{
    std::ifstream file(path);
    if(!file.is_open())
    {
        ff = true;
        return;
    }

    std::string line;
    while(std::getline(file, line))
    {
        std::string no, tx;
        bool getting = true;
        for(std::size_t i = 0; i < line.length(); ++i)
        {
            if(std::isdigit(line[i]) && getting)
            {
                no += line[i];
            }
            else if(getting)
            {
                getting = false;
            }
            else
            {
                tx += line[i];
            }
        }

        if(!no.empty())
        {
            v.push_back({ pcx::lexical_cast<std::size_t>(no), tx });
        }
    }
}

void Comments::write(std::ostream &os) const
{
    for(std::size_t i = 0; i < v.size(); ++i)
    {
        os << v[i].position  << " " << v[i].text << "\n";
    }
}

bool Comments::save(const std::string &path) const
{
    std::ofstream os(path);
    if(!os.is_open())
    {
        return false;
    }

    write(os);

    return true;
}

std::string Comments::header_imp(const std::string &title)
{
    return pcx::str("--- ", title, " ", std::string(119 - (title.length() + 5 <= 119 ? title.length() + 5 : 0), '-'));
}
