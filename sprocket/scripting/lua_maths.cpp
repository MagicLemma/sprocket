#include "lua_maths.h"

#include <sprocket/Scripting/lua_script.h>
#include <sprocket/Scripting/lua_converter.h>

#include <lua.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace spkt {
namespace lua {
namespace {

void load_vec3_functions(lua::script& script)
{
    lua_State* L = script.native_handle();

    luaL_newmetatable(L, "vec3");

    lua_pushcfunction(L, [](lua_State* L) {
        glm::vec3* vec = (glm::vec3*)luaL_checkudata(L, 1, "vec3");
        std::string_view k = luaL_checkstring(L, 2);
        if (k == "x") {
            lua_pushnumber(L, vec->x);
        }
        else if (k == "y") {
            lua_pushnumber(L, vec->y);
        }
        else if (k == "z") {
            lua_pushnumber(L, vec->z);
        }
        else {
            lua_pushnil(L);
        }
        return 1;
    });
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, [](lua_State* L) {
        glm::vec3* vec = (glm::vec3*)luaL_checkudata(L, 1, "vec3");
        std::string_view k = luaL_checkstring(L, 2);
        if (k == "x") {
            vec->x = luaL_checknumber(L, 3);
            return 0;
        }
        else if (k == "y") {
            vec->y = luaL_checknumber(L, 3);
            return 0;
        }
        else if (k == "z") {
            vec->z = luaL_checknumber(L, 3);
            return 0;
        }
        return luaL_argerror(L, 2, lua_pushfstring(L, "Invalid option '%s'", k));
    });
    lua_setfield(L, -2, "__newindex");

    lua_pushcfunction(L, [](lua_State* L) {
        glm::vec3* self = (glm::vec3*)luaL_checkudata(L, 1, "vec3");
        glm::vec3* other = (glm::vec3*)luaL_checkudata(L, 2, "vec3");
        converter<glm::vec3>::push(L, *self + *other);
        return 1;
    });
    lua_setfield(L, -2, "__add");

    lua_pushcfunction(L, [](lua_State* L) {
        glm::vec3* self = (glm::vec3*)luaL_checkudata(L, 1, "vec3");
        glm::vec3* other = (glm::vec3*)luaL_checkudata(L, 2, "vec3");
        converter<glm::vec3>::push(L, *self - *other);
        return 1;
    });
    lua_setfield(L, -2, "__sub");

    lua_pushcfunction(L, [](lua_State* L) {
        glm::vec3* self = (glm::vec3*)luaL_checkudata(L, 1, "vec3");
        glm::vec3* other = (glm::vec3*)luaL_checkudata(L, 2, "vec3");
        converter<bool>::push(L, *self == *other);
        return 1;
    });
    lua_setfield(L, -2, "__eq");

    lua_pushcfunction(L, [](lua_State* L) {
        if (lua_isnumber(L, 1)) {
            float scalar = (float)luaL_checknumber(L, 1);
            glm::vec3 vec = *(glm::vec3*)luaL_checkudata(L, 2, "vec3");
            converter<glm::vec3>::push(L, scalar * vec);
        }
        else if (lua_isnumber(L, 2)) {
            glm::vec3 vec = *(glm::vec3*)luaL_checkudata(L, 1, "vec3");
            float scalar = (float)luaL_checknumber(L, 2);
            converter<glm::vec3>::push(L, scalar * vec);
        }
        else {
            glm::vec3 self = *(glm::vec3*)luaL_checkudata(L, 1, "vec3");
            glm::vec3 other = *(glm::vec3*)luaL_checkudata(L, 2, "vec3");
            converter<glm::vec3>::push(L, self * other);
        }
        return 1;
    });
    lua_setfield(L, -2, "__mul");

    lua_pushcfunction(L, [](lua_State* L) {
        float x = luaL_checknumber(L, 1);
        float y = luaL_checknumber(L, 2);
        float z = luaL_checknumber(L, 3);
        converter<glm::vec3>::push(L, {x, y, z});
        return 1;
    });
    lua_setfield(L, -2, "new");

    lua_setglobal(L, "vec3");
}

void load_vec2_functions(lua::script& script)
{
    lua_State* L = script.native_handle();

    luaL_newmetatable(L, "vec2");

    lua_pushcfunction(L, [](lua_State* L) {
        glm::vec2* vec = (glm::vec2*)luaL_checkudata(L, 1, "vec2");
        std::string_view k = luaL_checkstring(L, 2);
        if (k == "x") {
            lua_pushnumber(L, vec->x);
        }
        else if (k == "y") {
            lua_pushnumber(L, vec->y);
        }
        else {
            lua_pushnil(L);
        }
        return 1;
    });
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, [](lua_State* L) {
        glm::vec2* vec = (glm::vec2*)luaL_checkudata(L, 1, "vec2");
        std::string_view k = luaL_checkstring(L, 2);
        if (k == "x") {
            vec->x = luaL_checknumber(L, 3);
            return 0;
        }
        else if (k == "y") {
            vec->y = luaL_checknumber(L, 3);
            return 0;
        }
        return luaL_argerror(L, 2, lua_pushfstring(L, "Invalid option '%s'", k));
    });
    lua_setfield(L, -2, "__newindex");

    lua_pushcfunction(L, [](lua_State* L) {
        glm::vec2* self = (glm::vec2*)luaL_checkudata(L, 1, "vec2");
        glm::vec2* other = (glm::vec2*)luaL_checkudata(L, 2, "vec2");
        converter<glm::vec2>::push(L, *self + *other);
        return 1;
    });
    lua_setfield(L, -2, "__add");

    lua_pushcfunction(L, [](lua_State* L) {
        glm::vec2* self = (glm::vec2*)luaL_checkudata(L, 1, "vec2");
        glm::vec2* other = (glm::vec2*)luaL_checkudata(L, 2, "vec2");
        converter<glm::vec2>::push(L, *self - *other);
        return 1;
    });
    lua_setfield(L, -2, "__sub");

    lua_pushcfunction(L, [](lua_State* L) {
        glm::vec2* self = (glm::vec2*)luaL_checkudata(L, 1, "vec2");
        glm::vec2* other = (glm::vec2*)luaL_checkudata(L, 2, "vec2");
        converter<bool>::push(L, *self == *other);
        return 1;
    });
    lua_setfield(L, -2, "__eq");

    lua_pushcfunction(L, [](lua_State* L) {
        if (lua_isnumber(L, 1)) {
            float scalar = (float)luaL_checknumber(L, 1);
            glm::vec2 vec = *(glm::vec2*)luaL_checkudata(L, 2, "vec2");
            converter<glm::vec2>::push(L, scalar * vec);
        }
        else if (lua_isnumber(L, 2)) {
            glm::vec2 vec = *(glm::vec2*)luaL_checkudata(L, 1, "vec2");
            float scalar = (float)luaL_checknumber(L, 2);
            converter<glm::vec2>::push(L, scalar * vec);
        }
        else {
            glm::vec2 self = *(glm::vec2*)luaL_checkudata(L, 1, "vec2");
            glm::vec2 other = *(glm::vec2*)luaL_checkudata(L, 2, "vec2");
            converter<glm::vec2>::push(L, self * other);
        }
        return 1;
    });
    lua_setfield(L, -2, "__mul");

    lua_pushcfunction(L, [](lua_State* L) {
        float x = luaL_checknumber(L, 1);
        float y = luaL_checknumber(L, 2);
        converter<glm::vec2>::push(L, {x, y});
        return 1;
    });
    lua_setfield(L, -2, "new");

    lua_setglobal(L, "vec2");
}

}

void load_maths(lua::script& script)
{
    load_vec2_functions(script);
    load_vec3_functions(script);
}

}
}