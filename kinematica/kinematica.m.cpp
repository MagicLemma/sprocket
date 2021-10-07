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

    point(const glm::vec2& p, bool f = false) : position(p), prev_position(p), fixed(f) {}
};

struct stick
{
    point* a;
    point* b;
    float  length;
};

std::pair<std::vector<std::shared_ptr<point>>, std::vector<stick>>
make_rope(const std::vector<glm::vec2>& points)
{
    std::vector<std::shared_ptr<point>> ret_points;
    std::vector<stick>                  ret_sticks;

    point* prev = nullptr;
    for (const auto pos : points) {
        auto p = std::make_shared<point>(pos);
        ret_points.push_back(p);
        if (prev != nullptr) {
            ret_sticks.push_back({prev, p.get(), glm::length(pos - prev->position)});
        }
        prev = p.get();
    }
    ret_points.back()->fixed = true;
    return {ret_points, ret_sticks};
}

class app
{
    spkt::window* d_window;
    spkt::shape_renderer d_renderer;

    std::vector<std::shared_ptr<point>> d_points; // TODO: Should be unique_ptr
    std::vector<stick>                  d_sticks;

    bool d_running = false;
    bool d_zig_zag = true;

public:
    app(spkt::window* window)
        : d_window{window}
    {
        d_window->set_clear_colour({0.1, 0.1, 0.1});
        std::tie(d_points, d_sticks) = make_rope({
                {100.0, 200.0},
                {150.0, 200.0},
                {250.0, 200.0},
                {350.0, 200.0},
                {350.0, 300.0},
                {400.0, 300.0},
                {600.0, 150.0}
            });
            d_points.front()->fixed = true;
    }

    void on_update(double dt)
    {
        if (d_running) {
            for (auto& point : d_points) {
                if (!point->fixed) {
                    auto current = point->position;
                    point->position += 0.9995f * (point->position - point->prev_position);
                    point->position += 45.0f * 9.81f * (float)dt * (float)dt * glm::vec2(0.0, 1.0);
                    point->prev_position = current;
                }
            }

            for (std::size_t i = 0; i != 5; ++i) {
                for (auto& stick : d_sticks) {
                    auto stick_centre = (stick.a->position + stick.b->position) / 2.0f;
                    auto stick_dir = glm::normalize(stick.a->position - stick.b->position);
                    auto stick_length = glm::length(stick.a->position - stick.b->position);

                    if (stick_length > stick.length) {
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
    }

    void on_event(spkt::event& ev)
    {
        if (auto data = ev.get_if<spkt::keyboard_pressed_event>()) {
            if (data->key == spkt::Keyboard::ENTER) {
                d_running = !d_running;
            } else if (data->key == spkt::Keyboard::ESC) {
                d_running = false;
                d_zig_zag = !d_zig_zag;
                if (d_zig_zag) {
                    std::tie(d_points, d_sticks) = make_rope({
                        {100.0, 200.0},
                        {150.0, 200.0},
                        {250.0, 200.0},
                        {350.0, 200.0},
                        {350.0, 300.0},
                        {400.0, 300.0},
                        {600.0, 150.0}
                    });
                    d_points.front()->fixed = true;
                } else {
                    std::tie(d_points, d_sticks) = make_rope({
                        {100.0, 100.0},
                        {100.0, 125.0},
                        {100.0, 150.0},
                        {100.0, 175.0},
                        {100.0, 200.0},
                        {100.0, 225.0},
                        {125.0, 225.0},
                        {150.0, 225.0},
                        {175.0, 225.0},
                        {200.0, 225.0},
                        {200.0, 200.0},
                        {200.0, 175.0},
                        {200.0, 150.0},
                        {200.0, 125.0},
                        {200.0, 100.0}
                    });
                    d_points.front()->fixed = true;
                }
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
                {0.0, 0.0, 0.0, 1.0},
                {0.0, 0.0, 0.0, 1.0},
                2.0f
            );
        }
        for (const auto& point : d_points) {
            if (point->fixed) {
                d_renderer.draw_circle(point->position, {1.0, 1.0, 1.0, 1.0}, 4.0f);
            } else {
                d_renderer.draw_circle(point->position, {1.0, 0.0, 0.0, 1.0}, 2.0f);
            }
        }
        d_renderer.end_frame();
    }
};

}

int main()
{
    return spkt::run_app<km::app>("kinematica");
}