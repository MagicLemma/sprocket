#pragma once
#include <Sprocket/Graphics/mesh.h>
#include <Sprocket/Graphics/buffer.h>

namespace spkt {

void draw(spkt::static_mesh* mesh, spkt::buffer<model_instance>* instances = nullptr);
void draw(spkt::animated_mesh* mesh, spkt::buffer<model_instance>* instances = nullptr);

}