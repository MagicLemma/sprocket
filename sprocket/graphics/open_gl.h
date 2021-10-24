#pragma once
// Wrappers for OpenGL functions. Intended to be low level and used by other classes in the
// Sprocket/Graphics module rather than being used directly. These functions mostly lack a
// proper home and may be moved when appropriate.
#include <sprocket/Graphics/mesh.h>
#include <sprocket/Graphics/buffer.h>

namespace spkt {

void draw(const spkt::static_mesh& mesh, spkt::vertex_buffer<model_instance>* instances = nullptr);
void draw(const spkt::animated_mesh& mesh, spkt::vertex_buffer<model_instance>* instances = nullptr);

template <typename T>
concept bindable = requires(T t)
{
    { t.bind() };
    { t.unbind() };
};

template <bindable T>
class bind_guard
{
    const T* const d_object;

    bind_guard(const bind_guard&) = delete;
    bind_guard& operator=(const bind_guard&) = delete;
    bind_guard(bind_guard&&) = delete;
    bind_guard& operator=(bind_guard&&) = delete;

public:
    bind_guard(const T& t) : d_object(&t) { d_object.bind(); }
    ~bind_guard() { d_object.unbind(); }
};

}