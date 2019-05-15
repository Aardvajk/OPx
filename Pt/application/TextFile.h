#ifndef TEXTFILE_H
#define TEXTFILE_H

#include <string>
#include <vector>

class TextFile
{
public:
    explicit TextFile(const std::string &path);

    bool failed() const { return ff; }

    std::string operator[](std::size_t index) const { return index < v.size() ? v[index] : std::string(); }
    std::size_t size() const { return v.size(); }
    std::string path() const { return p; }

private:
    bool ff;
    std::string p;
    std::vector<std::string> v;
};

#endif // TEXTFILE_H
