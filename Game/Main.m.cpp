#include <Sprocket.h>

#include "Game.h"

using namespace Sprocket;

template <typename T>
constexpr std::string_view type_name_raw() {
    return __FUNCSIG__;
}

template <typename T>
constexpr std::string_view type_name() {
    // Calculate how many chars are needed to be stripped from the front and
    // back of the raw string by counting using a known type.
    constexpr std::string_view float_raw = type_name_raw<float>();
    constexpr std::string_view float_parsed = "float";

    constexpr std::size_t num_prefix_chars = float_raw.find(float_parsed);
    static_assert(num_prefix_chars >= 0);

    constexpr std::size_t num_suffix_chars = float_raw.size() - num_prefix_chars - float_parsed.size();
    static_assert(num_suffix_chars >= 0);

    std::string_view raw = type_name_raw<T>();
    raw.remove_prefix(num_prefix_chars);
    raw.remove_suffix(num_suffix_chars);
    return raw;
}

template <typename T>
constexpr std::size_t sdbm_type_hash()
{
    std::size_t hash = 0;
    for (const auto& c : type_name_raw<T>()) {
        hash = c + 65599 * hash;
    }
    return hash;
};

struct type_index
{
    const std::string_view name;
    const std::size_t      hash;

    type_index(std::string_view name, std::size_t hash)
        : name(name)
        , hash(hash)
    {}
};

struct foo
{
    const char* x;
    std::size_t y;
};

template <typename T>
type_index get_type_index()
{
    return {type_name<T>(), sdbm_type_hash<T>()};
}

using XX = int;

int main()
{
    //Sprocket::Window window("Game");
    //WorldLayer game(&window);
    //Sprocket::RunOptions options;
    //options.showFramerate = true;
    //return Sprocket::Run(game, window, options);
    
    auto index = get_type_index<Sprocket::ecs::Registry>();
    log::info("{}", index.name);
    log::info("{}", index.hash);
    log::info("{}", sizeof(foo));
}
