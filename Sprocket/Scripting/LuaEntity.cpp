#include "LuaEntity.h"
#include "LuaGlobals.h"
#include "ECS.h"
#include "Scene.h"
#include "InputProxy.h"
#include "Window.h"
#include "Components.h"

#include <lua.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace Sprocket {
namespace lua {

void register_scene_functions(lua_State* L)
{
    using Generator = cppcoro::generator<ecs::Entity>;
    using Iterator = typename Generator::iterator;

    static_assert(std::is_trivially_destructible_v<Iterator>);
    
    lua_register(L, "NewEntity", [](lua_State* L) {
        if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
        auto luaEntity = static_cast<ecs::Entity*>(lua_newuserdata(L, sizeof(ecs::Entity)));
        *luaEntity = GetScene(L)->Entities().New();
        return 1;
    });

    lua_register(L, "DeleteEntity", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        auto luaEntity = static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        luaEntity->Delete();
        return 0;
    });

    lua_register(L, "Each_New", [](lua_State* L) {
        if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
        auto gen = new Generator(GetScene(L)->Entities().Each());
        lua_pushlightuserdata(L, static_cast<void*>(gen));
        return 1;
    });

    lua_register(L, "Each_Delete", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        delete static_cast<Generator*>(lua_touserdata(L, 1));
        return 0;
    });

    lua_register(L, "Each_Iter_Start", [](lua_State*L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        auto gen = static_cast<Generator*>(lua_touserdata(L, 1));

        auto iter = static_cast<Iterator*>(lua_newuserdata(L, sizeof(Iterator)));
        *iter = gen->begin();
        return 1;
    });

    lua_register(L, "Each_Iter_Valid", [](lua_State* L) {
        if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }
        auto gen = static_cast<Generator*>(lua_touserdata(L, 1));
        auto iter = static_cast<Iterator*>(lua_touserdata(L, 2));

        lua_pushboolean(L, *iter != gen->end());
        return 1;
    });

    lua_register(L, "Each_Iter_Next", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        auto iter = static_cast<Iterator*>(lua_touserdata(L, 1));
        ++(*iter);
        return 0;   
    });

    lua_register(L, "Each_Iter_Get", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        auto iterator = static_cast<Iterator*>(lua_touserdata(L, 1));

        auto luaEntity = static_cast<ecs::Entity*>(lua_newuserdata(L, sizeof(ecs::Entity)));
        *luaEntity = **iterator;
        return 1;
    });
}

void register_entity_transformation_functions(lua_State* L)
{
    lua_register(L, "SetLookAt", [](lua_State* L) {
        if (!CheckArgCount(L, 7)) { return luaL_error(L, "Bad number of args"); }

        ecs::Entity entity = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        float px = (float)lua_tonumber(L, 2);
        float py = (float)lua_tonumber(L, 3);
        float pz = (float)lua_tonumber(L, 4);

        float tx = (float)lua_tonumber(L, 5);
        float ty = (float)lua_tonumber(L, 6);
        float tz = (float)lua_tonumber(L, 7);

        auto& tr = entity.Get<Transform3DComponent>();
        tr.position = glm::vec3(px, py, pz);
        tr.orientation = glm::conjugate(glm::quat_cast(glm::lookAt(tr.position, {tx, ty, tz}, {0.0, 1.0, 0.0})));
        return 0;
    });

    lua_register(L, "RotateY", [](lua_State* L) {
        if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); };

        ecs::Entity entity = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& tr = entity.Get<Transform3DComponent>();

        float yaw = (float)lua_tonumber(L, 2);
        tr.orientation = glm::rotate(tr.orientation, yaw, {0, 1, 0});
        return 0;
    });

    lua_register(L, "GetForwardsDir", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        ecs::Entity entity = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& tr = entity.Get<Transform3DComponent>();
        auto o = tr.orientation;
        
        if (entity.Has<Camera3DComponent>()) {
            auto pitch = entity.Get<Camera3DComponent>().pitch;
            o = glm::rotate(o, pitch, {1, 0, 0});
        }

        auto forwards = Maths::Forwards(o);

        lua_pushnumber(L, forwards.x);
        lua_pushnumber(L, forwards.y);
        lua_pushnumber(L, forwards.z);
        return 3;
    });

    lua_register(L, "GetRightDir", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        ecs::Entity entity = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& tr = entity.Get<Transform3DComponent>();
        auto right = Maths::Right(tr.orientation);
        lua_pushnumber(L, right.x);
        lua_pushnumber(L, right.y);
        lua_pushnumber(L, right.z);
        return 3;
    });

    lua_register(L, "MakeUpright", [](lua_State* L) {
        if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }
        ecs::Entity entity = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& tr = entity.Get<Transform3DComponent>();
        float yaw = (float)lua_tonumber(L, 2);
        tr.orientation = glm::quat(glm::vec3(0, yaw, 0));
        return 0;
    });
}

void register_input_functions(lua_State* L)
{
    lua_register(L, "IsKeyDown", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        if (auto ip = GetInput(L); ip) {
            int x = (int)lua_tointeger(L, 1);
            lua_pushboolean(L, ip->IsKeyboardDown(x));
        }
        else {
            lua_pushboolean(L, false);
        }
        
        return 1;
    });
    
    lua_register(L, "IsMouseDown", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        if (auto ip = GetInput(L); ip) {
            int x = (int)lua_tointeger(L, 1);
            lua_pushboolean(L, ip->IsMouseDown(x));
        }
        else {
            lua_pushboolean(L, false);
        }

        return 1;
    });
    
    lua_register(L, "GetMousePos", [](lua_State* L) {
        if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }

        if (auto w = GetWindow(L); w) {
            auto offset = w->GetMousePos();
            lua_pushnumber(L, offset.x);
            lua_pushnumber(L, offset.y);
        } else {
            lua_pushnumber(L, 0);
            lua_pushnumber(L, 0);
        }
        return 2;
    });

    lua_register(L, "GetMouseOffset", [](lua_State* L) {
        if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }

        if (auto w = GetWindow(L); w) {
            auto offset = w->GetMouseOffset();
            lua_pushnumber(L, offset.x);
            lua_pushnumber(L, offset.y);
        } else {
            lua_pushnumber(L, 0);
            lua_pushnumber(L, 0);
        }
        return 2;
    });
}

}
}