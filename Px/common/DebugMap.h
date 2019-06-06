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
        std::string name;
        std::size_t size;
        std::vector<Comment> comments;
    };

    using iterator = std::vector<Entity>::iterator;
    using const_iterator = std::vector<Entity>::const_iterator;
    using value_type = std::vector<Entity>::value_type;

    DebugMap();

    using Callback = pcx::callback<std::size_t>;

    void push_back(const Entity &value){ v.push_back(value); }

    void begin(char type, const std::string &name, std::size_t size, pcx::optional<Callback> callback);

    template<typename... Args> void operator()(Args&&... args);

    void read(std::istream &is);
    void write(std::ostream &os) const;

    Entity &operator[](std::size_t index){ return v[index]; }
    const Entity &operator[](std::size_t index) const { return v[index]; }

    iterator begin(){ return v.begin(); }
    iterator end(){ return v.end(); }

    const_iterator begin() const { return v.begin(); }
    const_iterator end() const { return v.end(); }

    Entity &back(){ return v.back(); }
    const Entity &back() const { return v.back(); }

    std::size_t size() const { return v.size(); }
    bool empty() const { return v.empty(); }

private:
    std::vector<Entity> v;
    pcx::optional<Callback> cb;
};

template<typename... Args> void DebugMap::operator()(Args&&... args)
{
    v.back().comments.push_back({ cb ? (*cb)() : 0, pcx::str(std::forward<Args>(args)...) });
}

#endif // DEBUGMAP_H
