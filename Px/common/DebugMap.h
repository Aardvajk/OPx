#ifndef DEBUGMAP_H
#define DEBUGMAP_H

#include <pcx/optional.h>
#include <pcx/callback.h>
#include <pcx/str.h>

#include <vector>
#include <string>
#include <iostream>

class DebugMap
{
public:
    struct Comment
    {
        std::size_t address;
        std::string text;
    };

    struct Entity
    {
        char type;
        std::size_t size;
        std::vector<Comment> comments;
    };

    DebugMap();

    using Callback = pcx::callback<std::size_t>;

    void begin(char type, std::size_t size, pcx::optional<Callback> callback);

    template<typename... Args> void operator()(Args&&... args);

    void read(std::istream &is);
    void write(std::ostream &os) const;

    Entity &operator[](std::size_t index){ return v[index]; }
    const Entity &operator[](std::size_t index) const { return v[index]; }

    std::size_t size() const { return v.size(); }

private:
    std::vector<Entity> v;
    pcx::optional<Callback> cb;
};

template<typename... Args> void DebugMap::operator()(Args&&... args)
{
    v.back().comments.push_back({ cb ? (*cb)() : 0, pcx::str(std::forward<Args>(args)...) });
}

#endif // DEBUGMAP_H
