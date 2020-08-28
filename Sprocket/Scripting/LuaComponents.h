// GENERATED FILE @ 2020-08-28 16:33:03.956530
class lua_State;

namespace Sprocket {

void RegisterComponentFunctions(lua_State* L);
    // Register all the functions in this header with the given lua state.

namespace Lua {

int GetNameComponent(lua_State* L);
int SetNameComponent(lua_State* L);

int GetTransformComponent(lua_State* L);
int SetTransformComponent(lua_State* L);

int GetModelComponent(lua_State* L);
int SetModelComponent(lua_State* L);

int GetRigidBody3DComponent(lua_State* L);
int SetRigidBody3DComponent(lua_State* L);

int GetBoxCollider3DComponent(lua_State* L);
int SetBoxCollider3DComponent(lua_State* L);

int GetSphereCollider3DComponent(lua_State* L);
int SetSphereCollider3DComponent(lua_State* L);

int GetCapsuleCollider3DComponent(lua_State* L);
int SetCapsuleCollider3DComponent(lua_State* L);

int GetCameraComponent(lua_State* L);
int SetCameraComponent(lua_State* L);

int GetSelectComponent(lua_State* L);
int SetSelectComponent(lua_State* L);

int GetPathComponent(lua_State* L);
int SetPathComponent(lua_State* L);

int GetGridComponent(lua_State* L);
int SetGridComponent(lua_State* L);

int GetLightComponent(lua_State* L);
int SetLightComponent(lua_State* L);

}
}