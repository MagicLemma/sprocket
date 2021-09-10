#pragma once
#include <Sprocket/Graphics/Material.h>
#include <Sprocket/Graphics/mesh.h>
#include <Sprocket/Graphics/Texture.h>

#include <filesystem>
#include <future>
#include <concepts>
#include <memory>
#include <ranges>
#include <string_view>
#include <string>
#include <unordered_map>
#include <tuple>

namespace spkt {

template <typename T>
concept loadable = requires(const std::string& file)
{
    requires std::default_initializable<T>;
    { T::load(file) } -> std::convertible_to<T>;
};

template <loadable T>
class single_asset_manager
{
public:
    using asset_type = T;
    using data_type = decltype(T::load(std::declval<std::string>()));

private:
    mutable std::unordered_map<std::string, std::future<data_type>> d_loading;
    mutable std::unordered_map<std::string, asset_type>             d_assets;
    asset_type                                                      d_default;

public:
    bool is_loading() const { return !d_loading.empty(); }
    auto view() const { return std::views::all(d_assets); }

    asset_type& get(std::string_view file)
    {
        if (file == "") { return d_default; }
        std::string filepath = std::filesystem::absolute(file).string();

        if (auto it = d_assets.find(filepath); it != d_assets.end()) {
            return it->second;
        }

        if (auto it = d_loading.find(filepath); it != d_loading.end()) {
            if (it->second.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                auto rc = d_assets.emplace(filepath, it->second.get());
                d_loading.erase(it);
                return rc.first->second;
            }
        } else {
            const auto loader = [filepath]() { return T::load(filepath); };
            d_loading.emplace(filepath, std::async(std::launch::async, loader));
        }

        return d_default;
    }

    const asset_type& get(std::string_view file) const { return get(file); }
};

template <typename... Ts>
class basic_asset_manager
{
private:
    std::tuple<spkt::single_asset_manager<Ts>...> d_managers;

    template <typename T>
    spkt::single_asset_manager<T>& manager()
    {
        return std::get<spkt::single_asset_manager<T>>(d_managers);
    }
    
    template <typename T>
    const spkt::single_asset_manager<T>& manager() const
    {
        return std::get<spkt::single_asset_manager<T>>(d_managers);
    }

public:
    template <typename T>
    bool is_loading() const { return manager<T>().is_loading(); }

    template <typename T>
    auto view() const { return manager<T>().view(); }

    template <typename T>
    decltype(auto) get(std::string_view file) { return manager<T>().get(file); }

    template <typename T>
    decltype(auto) get(std::string_view file) const { return manager<T>().get(file); }

    bool is_loading_anything() const { return (manager<Ts>().is_loading() || ...); }
};

using asset_manager = spkt::basic_asset_manager<
    static_mesh,
    animated_mesh,
    texture,
    Material
>;

}