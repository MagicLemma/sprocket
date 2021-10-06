#include <Sprocket/core.h>
#include <Sprocket/Graphics/Rendering/shape_renderer.h>

#include <glm/glm.hpp>

#include <vector>
#include <memory>

namespace km {

struct point
{
    glm::vec2 position;
    glm::vec2 prev_position;
    bool      fixed;

    //glm::vec2 velocity = {0.0, 0.0};

    point(const glm::vec2& p, bool f = false) : position(p), prev_position(p), fixed(f) {}
};

struct stick
{
    point* a;
    point* b;
    float  length;
};

class app
{
    spkt::window* d_window;
    spkt::shape_renderer d_renderer;

    std::vector<std::shared_ptr<point>> d_points; // TODO: Should be unique_ptr
    std::vector<stick>                  d_sticks;

    bool d_running = false;

public:
    app(spkt::window* window)
        : d_window{window}
        , d_points{
            std::make_shared<point>(glm::vec2{100.0, 100.0}),
            std::make_shared<point>(glm::vec2{200.0, 100.0}),
            std::make_shared<point>(glm::vec2{300.0, 100.0}),
            std::make_shared<point>(glm::vec2{400.0, 100.0}, true)
        },
        d_sticks{
            {d_points[0].get(), d_points[1].get(), 100.0f},
            {d_points[1].get(), d_points[2].get(), 100.0f},
            {d_points[2].get(), d_points[3].get(), 100.0f}
        }
    {}

    void on_update(double dt)
    {
        if (d_running) {
            for (auto& point : d_points) {
                if (!point->fixed) {
                    auto current = point->position;
                    point->position += point->position - point->prev_position;
                    point->position += 50.0f * 9.81f * (float)dt * (float)dt * glm::vec2(0.0, 1.0);
                    point->prev_position = current;
                }
            }

            for (std::size_t i = 0; i != 10; ++i) {
                for (auto& stick : d_sticks) {
                    auto stick_centre = (stick.a->position + stick.b->position) / 2.0f;
                    auto stick_dir = glm::normalize(stick.a->position - stick.b->position);
                    if (!stick.a->fixed) {
                        stick.a->position = stick_centre + stick_dir * stick.length / 2.0f;
                    }
                    if (!stick.b->fixed) {
                        stick.b->position = stick_centre - stick_dir * stick.length / 2.0f;
                    }
                }
            }
        }
    }

    void on_event(spkt::event& ev)
    {
        if (auto data = ev.get_if<spkt::keyboard_pressed_event>()) {
            if (data->key == spkt::Keyboard::ENTER) {
                d_running = !d_running;
            }
        }
    }

    void on_render()
    {
        d_renderer.begin_frame((float)d_window->width(), (float)d_window->height());
        for (const auto& stick : d_sticks) {
            d_renderer.draw_line(
                stick.a->position,
                stick.b->position,
                {0.0, 0.0, 1.0, 1.0},
                {0.0, 0.0, 1.0, 1.0},
                2.0f
            );
        }
        for (const auto& point : d_points) {
            glm::vec4 colour{point->fixed ? 0.0 : 1.0, 0.0, 0.0, 1.0};
            d_renderer.draw_circle(point->position, colour, 10.0f);
        }
        d_renderer.end_frame();
    }
};

}

int main()
{
    return spkt::run_app<km::app>("kinematica");
}