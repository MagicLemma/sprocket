#include "Graphics/Displays/DisplayRenderer.h"
#include "Utility/Maths.h"
#include "Log.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Sprocket {

DisplayRenderer::DisplayRenderer(Window* window)
    : d_window(window)
    , d_shader("Resources/Shaders/Display.vert",
               "Resources/Shaders/Display.frag")
    , d_quadModel(Model2D({
        {Sprocket::Maths::vec2{0.0f, 1.0f}, Sprocket::Maths::vec2{1.0f, 1.0f}},
        {Sprocket::Maths::vec2{1.0f, 1.0f}, Sprocket::Maths::vec2{1.0f, 0.0f}},
        {Sprocket::Maths::vec2{0.0f, 0.0f}, Sprocket::Maths::vec2{0.0f, 1.0f}},
        {Sprocket::Maths::vec2{1.0f, 0.0f}, Sprocket::Maths::vec2{0.0f, 0.0f}}
    }))
{
}

void DisplayRenderer::draw(const Quad& quad) const
{
    handleRenderOptions({false, false, false});
    d_shader.bind();
    d_quadModel.bind();

    Maths::mat4 projection = Maths::ortho(0, (float)d_window->width(),
                                          (float)d_window->height(), 0);

    Maths::mat4 transform = Maths::transform(quad.topLeftV3(), {0.0, 0.0, 0.0}, {quad.width(), quad.height(), 0.0});

    d_shader.loadUniform("projection", projection);
    d_shader.loadUniform("transform", transform);
    d_shader.loadUniform("colour", quad.colour());

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    d_quadModel.unbind();
    d_shader.unbind();   
}

void DisplayRenderer::draw(const Slider& slider) const
{
    handleRenderOptions({false, false, false});
    d_shader.bind();
    d_quadModel.bind();

    Maths::mat4 projection = Maths::ortho(0, (float)d_window->width(),
                                          (float)d_window->height(), 0);
    d_shader.loadUniform("projection", projection);

    // Background quad
    auto quad = slider.background();
    Maths::mat4 transform = Maths::transform(quad.topLeftV3(), {0.0, 0.0, 0.0}, {quad.width(), quad.height(), 0.0});
    d_shader.loadUniform("transform", transform);
    d_shader.loadUniform("colour", quad.colour());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Bar quad
    quad = slider.bar();
    transform = Maths::transform(quad.topLeftV3(), {0.0, 0.0, 0.0}, {quad.width(), quad.height(), 0.0});
    d_shader.loadUniform("transform", transform);
    d_shader.loadUniform("colour", quad.colour());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Picker quad
    quad = slider.picker();
    transform = Maths::transform(quad.topLeftV3(), {0.0, 0.0, 0.0}, {quad.width(), quad.height(), 0.0});
    d_shader.loadUniform("transform", transform);
    d_shader.loadUniform("colour", quad.colour());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    d_quadModel.unbind();
    d_shader.unbind();
}

void DisplayRenderer::draw(const Button& button) const
{
    handleRenderOptions({false, false, false});
    d_shader.bind();
    d_quadModel.bind();

    Maths::mat4 projection = Maths::ortho(0, (float)d_window->width(),
                                          (float)d_window->height(), 0);
    d_shader.loadUniform("projection", projection);

    // Background quad
    auto quad = button.background();
    Maths::mat4 transform = Maths::transform(quad.topLeftV3(), {0.0, 0.0, 0.0}, {quad.width(), quad.height(), 0.0});
    d_shader.loadUniform("transform", transform);
    d_shader.loadUniform("colour", quad.colour());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Button quad
    quad = button.button();
    transform = Maths::transform(quad.topLeftV3(), {0.0, 0.0, 0.0}, {quad.width(), quad.height(), 0.0});
    d_shader.loadUniform("transform", transform);
    d_shader.loadUniform("colour", quad.colour());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    d_quadModel.unbind();
    d_shader.unbind();
}

}