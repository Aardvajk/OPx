#ifndef COMMENTS_H
#define COMMENTS_H

#include <vector>
#include <string>
#include <utility>

#include <pcx/str.h>
#include <pcx/optional.h>
#include <pcx/callback.h>

class Comments
{
public:
    struct Item
    {
        std::size_t position;
        std::string text;
    };

    struct Bare_T { };
    static const Bare_T Bare;

    Comments();
    explicit Comments(pcx::callback<std::size_t> cb);
    explicit Comments(const std::string &path);

    bool failed() const { return ff; }

    void write(std::ostream &os) const;
    bool save(const std::string &path) const;

    template<typename... Args> Comments &operator()(Args&&... args);
    template<typename... Args> Comments &operator()(Bare_T, Args&&... args);

    Item operator[](std::size_t index) const { return v[index]; }
    std::size_t size() const { return v.size(); }

private:
    static std::string header_imp(const std::string &title);

    bool ff;
    pcx::optional<pcx::callback<std::size_t> > cb;
    std::vector<Item> v;
};

template<typename... Args> Comments &Comments::operator()(Args&&... args)
{
    v.push_back({ cb ? (*cb)() : std::size_t(0), header_imp(pcx::str(std::forward<Args>(args)...) )});
    return *this;
}

template<typename... Args> Comments &Comments::operator()(Bare_T, Args&&... args)
{
    v.push_back({ cb ? (*cb)() : std::size_t(0), pcx::str(std::forward<Args>(args)...) });
    return *this;
}

#endif // COMMENTS_H
