#pragma once
#include "Event.h"
#include "MouseCodes.h"
#include "KeyboardCodes.h"

#include <array>
#include <tsl/hopscotch_map.h>
#include <tsl/hopscotch_set.h>

namespace Sprocket {

class InputProxy
{
    // Mouse
    std::array<bool, 5> d_buttons;

    // Keyboard
    tsl::hopscotch_map<int, bool> d_keys;
    tsl::hopscotch_set<int> d_consumedKeys;
    bool d_consumeAll = true;

public:

    void OnEvent(Event& event);

    bool IsMouseDown(int button) const;
    bool IsKeyboardDown(int key) const;

    void ConsumeAll(bool value);
    void ConsumeEventsFor(int key);
};
    
}