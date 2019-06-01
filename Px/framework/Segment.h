#ifndef SEGMENT_H
#define SEGMENT_H

#include <vector>

class InputStream;

class Segment
{
public:
    Segment();

    void read(std::size_t bytes, InputStream &is);

    char &operator[](std::size_t i){ return v[i]; }
    char operator[](std::size_t i) const { return v[i]; }

    const char *data() const { return v.data(); }
    std::size_t size() const { return v.size(); }

    std::vector<char>::iterator begin(){ return v.begin(); }
    std::vector<char>::iterator end(){ return v.end(); }

    std::vector<char>::const_iterator begin() const { return v.begin(); }
    std::vector<char>::const_iterator end() const { return v.end(); }

private:
    std::vector<char> v;
};

#endif // SEGMENT_H
