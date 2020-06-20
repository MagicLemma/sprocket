#pragma once

class lua_State;

namespace Sprocket {

void RegisterInputFunctions(lua_State* L);

namespace Lua {

// Functions that will be reigstered with all Lua engines for accessing
// the keyboard and mouse for querying what buttons and keys are pressed.

int IsKeyDown(lua_State* L);
    // Args: 1 - The keyboard key (int code).
    // Rets: 1 - A boolean that is true if the key is pressed.

int IsMouseDown(lua_State* L);
    // Args: 1 - The mouse button (int code).
    // Rets: 1 - A boolean that is true if the button is pressed.

int GetMousePos(lua_State* L);
    // Args: 0
    // Rets: 2 - The x, y coordinates of the mouse position.

int GetMouseOffset(lua_State* L);
    // Args: 0
    // Rets: 2 - The x, y offsets of the mouse from last frame.

}
}