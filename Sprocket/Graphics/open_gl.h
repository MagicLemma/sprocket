#pragma once
// Wrappers for OpenGL functions. Intended to be low level and used by other classes in the
// Sprocket/Graphics module rather than being used directly. These functions mostly lack a
// proper home and may be moved when appropriate.
#include <Sprocket/Graphics/mesh.h>
#include <Sprocket/Graphics/buffer.h>

namespace spkt {

void draw(spkt::static_mesh* mesh, spkt::buffer<model_instance>* instances = nullptr);
void draw(spkt::animated_mesh* mesh, spkt::buffer<model_instance>* instances = nullptr);

}