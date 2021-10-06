#include <Sprocket/core.h>

namespace km {

class app
{
    spkt::window* d_window;

public:
    app(spkt::window* window) : d_window(window) {}

    void on_update(double dt) {}
    void on_event(spkt::event& ev) {}
    void on_render() {}
};

}

int main()
{
    spkt::window window("Game");
    km::app game(&window);
    spkt::run_options options;
    options.show_frame_rate = true;
    return spkt::run(game, window, options);
    return 0;
}