#include "open_gl.h"

#include <glad/glad.h>

#include <concepts>

namespace spkt {
namespace {

template <typename T>
concept bindable = requires(T t)
{
    { t.bind() } -> std::same_as<void>;
    { t.vertex_count() } -> std::same_as<std::size_t>;
};

template <bindable T>
void draw_impl(const T& mesh, spkt::vertex_buffer<model_instance>* instances)
{
    mesh.bind();
    if (instances) {
        instances->bind();
        glDrawElementsInstanced(GL_TRIANGLES, (int)mesh.vertex_count(), GL_UNSIGNED_INT, nullptr, instances->size());
    } else {
        glDrawElements(GL_TRIANGLES, (int)mesh.vertex_count(), GL_UNSIGNED_INT, nullptr);
    }
}

}

void draw(const spkt::static_mesh& mesh, spkt::vertex_buffer<model_instance>* instances)
{
    draw_impl(mesh, instances);
}

void draw(const spkt::animated_mesh& mesh, spkt::vertex_buffer<model_instance>* instances)
{
    draw_impl(mesh, instances);
}

}