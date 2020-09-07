class lua_State;

namespace Sprocket {

void RegisterComponentFunctions(lua_State* L);
    // Register all the functions in this header with the given lua state.

namespace Lua {

#ifdef DATAMATIC_BLOCK
int Get{{Comp.Name}}(lua_State* L);
int Set{{Comp.Name}}(lua_State* L);
int Add{{Comp.Name}}(lua_State* L);

#endif

}
}