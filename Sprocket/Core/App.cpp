#include "App.h"
#include "Adaptors.h"

namespace Sprocket {

void App::OnEvent(Event& event)
{
    for (auto& layer : Reversed(d_layers)) {
        layer->OnEvent(event);
    }
}

void App::OnUpdate(double dt)
{ 
    for (auto& layer : d_layers) {
        layer->OnUpdate(dt);
    }
}

}