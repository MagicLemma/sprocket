#pragma once
#include "Window.h"
#include "Event.h"

#include <memory>

namespace Sprocket {
namespace SimpleUI {

struct SimpleUIData;

class Context
{
    std::shared_ptr<SimpleUIData> d_impl;

public:
    Context(Window* window);

    void OnEvent(Event& event);
    void OnUpdate(double dt);
};

}
}