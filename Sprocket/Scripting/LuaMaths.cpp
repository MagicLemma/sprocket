#include "LuaScript.h"

#include <lua.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace Sprocket {
namespace lua {
namespace {

glm::vec3* vec3_new(lua_State* L)
{
    glm::vec3* vec = (glm::vec3*)lua_newuserdata(L, sizeof(glm::vec3));
    luaL_setmetatable(L, "vec3");
    return vec;
}

glm::vec3* vec3_new(lua_State* L, float x, float y, float z)
{
    glm::vec3* vec = vec3_new(L);
    vec->x = x;
    vec->y = y;
    vec->z = z;
    return vec;
}

glm::vec3* vec3_new(lua_State* L, const glm::vec3& value)
{
    glm::vec3* vec = vec3_new(L);
    *vec = value;
    return vec;
}

}

void load_vec3_functions(lua::Script& script)
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
        glm::vec3* result = vec3_new(L, *self + *other);
        return 1;
    });
    lua_setfield(L, -2, "__add");

    lua_pushcfunction(L, [](lua_State* L) {
        glm::vec3* self = (glm::vec3*)luaL_checkudata(L, 1, "vec3");
        glm::vec3* other = (glm::vec3*)luaL_checkudata(L, 2, "vec3");
        glm::vec3* result = vec3_new(L, *self - *other);
        return 1;
    });
    lua_setfield(L, -2, "__sub");

    lua_pushcfunction(L, [](lua_State* L) {
        if (lua_isnumber(L, 1)) {
            float scalar = (float)luaL_checknumber(L, 1);
            glm::vec3* vec = (glm::vec3*)luaL_checkudata(L, 2, "vec3");
            vec3_new(L, scalar * vec->x, scalar * vec->y, scalar * vec->z);
        }
        else if (lua_isnumber(L, 2)) {
            glm::vec3* vec = (glm::vec3*)luaL_checkudata(L, 1, "vec3");
            float scalar = (float)luaL_checknumber(L, 2);
            vec3_new(L, scalar * vec->x, scalar * vec->y, scalar * vec->z);
        }
        else {
            glm::vec3* self = (glm::vec3*)luaL_checkudata(L, 1, "vec3");
            glm::vec3* other = (glm::vec3*)luaL_checkudata(L, 2, "vec3");
            vec3_new(L, self->x * other->x, self->y * other->y, self->z * other->z);
        }
        return 1;
    });
    lua_setfield(L, -2, "__mul");

    lua_pushcfunction(L, [](lua_State* L) {
        float x = luaL_checknumber(L, 1);
        float y = luaL_checknumber(L, 2);
        float z = luaL_checknumber(L, 3);
        vec3_new(L, x, y, z);
        return 1;
    });
    lua_setfield(L, -2, "new");

    lua_setglobal(L, "vec3");
}

}
}