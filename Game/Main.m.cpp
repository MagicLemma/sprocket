#include <Sprocket.h>
#include "Game.h"

int main()
{
#ifdef GAME
    Sprocket::Window window("Game");
    WorldLayer game(&window);
    Sprocket::RunOptions options;
    options.showFramerate = true;
    return Sprocket::Run(game, window, options);
#else
    Sprocket::Log::Init();
    Sprocket::ECS::Registry reg;

    auto e = reg.New();
    e.Emplace<Sprocket::NameComponent>();
    e = reg.New();
    e.Emplace<Sprocket::NameComponent>();
    e = reg.New();
    e.Emplace<Sprocket::NameComponent>();
    e = reg.New();
    e.Emplace<Sprocket::NameComponent>();
    e = reg.New();
    e.Emplace<Sprocket::NameComponent>();

    int i = 0;
    for (auto entity : reg.View<Sprocket::NameComponent>()) {
        if (i == 2) {
            SPKT_LOG_INFO("Deleting index {} version {}", entity.Slot(), entity.Version());
            reg.Delete(entity);
        }
        ++i;
    }

    i = 0;
    for (auto entity : reg.View<Sprocket::NameComponent>()) {
        if (i == 3) {
            auto e = reg.New();
            SPKT_LOG_INFO("Created index {} version {}", e.Slot(), e.Version());
            e.Emplace<Sprocket::TransformComponent>();
        }
        ++i;
    }


#endif
}
