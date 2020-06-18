#pragma once
#include "Event.h"

#include <unordered_map>
#include <unordered_set>

namespace Sprocket {

class KeyboardProxy
// Rather than objects querying the window to check if keys are pressed
// or not, a KeyboardProxy may be used instead. This will update on
// non-consumed button pressed events and all button released events.
// The main motivation for this was that typing WASD into a text box would
// still make the camera move.
{
    std::unordered_map<int, bool> d_pressedKeys;
    std::unordered_set<int> d_consumedKeys;
    bool d_consumeAll = true;

public:
    void OnEvent(Event& event);
    bool IsKeyDown(int key) const;

    // TODO: Remove ConsumeAll and make non consumed by default.
    void ConsumeAll(bool value);
    void ConsumeEventsFor(int key);
};

}