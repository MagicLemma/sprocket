#include "LuaTransform.h"
#include "LuaGlobals.h"
#include "Entity.h"
#include "Log.h"
#include "Components.h"

#include <lua.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace Sprocket {

void RegisterTransformFunctions(lua_State* L)
{
    // Most of these are prefixed with Lua_ as they get wrapped in the
    // Lua world so the args and return values are Vec3 objects. See
    // Sprocket.lua for the wrappings.
    lua_register(L, "Lua_SetLookAt", &Lua::SetLookAt);
    
    lua_register(L, "Lua_GetForwardsDir", &Lua::GetForwardsDir);
    lua_register(L, "Lua_GetRightDir", &Lua::GetRightDir);

    lua_register(L, "RotateY", &Lua::RotateY);
    lua_register(L, "MakeUpright", &Lua::MakeUpright);
}

namespace Lua {

int SetLookAt(lua_State* L)
{
    if (!CheckArgCount(L, 7)) { return luaL_error(L, "Bad number of args"); }

    Entity entity = *static_cast<Entity*>(lua_touserdata(L, 1));
    float px = (float)lua_tonumber(L, 2);
    float py = (float)lua_tonumber(L, 3);
    float pz = (float)lua_tonumber(L, 4);

    float tx = (float)lua_tonumber(L, 5);
    float ty = (float)lua_tonumber(L, 6);
    float tz = (float)lua_tonumber(L, 7);

    auto& tr = entity.Get<TransformComponent>();
    tr.position = {px, py, pz};
    tr.orientation = glm::conjugate(glm::quat_cast(glm::lookAt(tr.position, {tx, ty, tz}, {0.0, 1.0, 0.0})));
    return 0;
}

int RotateY(lua_State* L)
{
    if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); };

    Entity entity = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& tr = entity.Get<TransformComponent>();

    float yaw = (float)lua_tonumber(L, 2);
    tr.orientation = Maths::Rotate(tr.orientation, {0, 1, 0}, glm::radians(yaw));
    return 0;
}

int GetForwardsDir(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    Entity entity = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& tr = entity.Get<TransformComponent>();
    auto o = tr.orientation;
    
    if (entity.Has<CameraComponent>()) {
        auto pitch = entity.Get<CameraComponent>().pitch;
        o *= Maths::Rotate({1, 0, 0}, pitch);
    }

    auto forwards = Maths::Forwards(o);

    lua_pushnumber(L, forwards.x);
    lua_pushnumber(L, forwards.y);
    lua_pushnumber(L, forwards.z);
    return 3;
}

int GetRightDir(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    Entity entity = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& tr = entity.Get<TransformComponent>();
    auto right = Maths::Right(tr.orientation);
    lua_pushnumber(L, right.x);
    lua_pushnumber(L, right.y);
    lua_pushnumber(L, right.z);
    return 3;
}

int MakeUpright(lua_State* L)
{
    if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }
    Entity entity = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& tr = entity.Get<TransformComponent>();
    float yaw = (float)lua_tonumber(L, 2);
    tr.orientation = glm::quat(glm::vec3(0, yaw, 0));
    return 0;
}

}
}