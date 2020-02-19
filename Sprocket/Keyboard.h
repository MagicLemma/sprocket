#pragma once
#include "Window.h"

namespace Sprocket {
namespace Keyboard {

void init(Window* window);
    // Initialises the Keyboard module by reigstering a callback with the
    // window. This is how the Keyboard module will get updated.

bool isKeyPressed(int key);
    // Returns true if the key is currently pressed and false otherwise.

enum Key {
    W = 87,
    A = 65,
    S = 83,
    D = 68
};

}
}