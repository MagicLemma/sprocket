// GENERATED FILE
class lua_State;

namespace Sprocket {

void RegisterComponentFunctions(lua_State* L);
    // Register all the functions in this header with the given lua state.

namespace Lua {

int GetNameComponent(lua_State* L);
int SetNameComponent(lua_State* L);
int AddNameComponent(lua_State* L);

int GetTransformComponent(lua_State* L);
int SetTransformComponent(lua_State* L);
int AddTransformComponent(lua_State* L);

int GetModelComponent(lua_State* L);
int SetModelComponent(lua_State* L);
int AddModelComponent(lua_State* L);

int GetRigidBody3DComponent(lua_State* L);
int SetRigidBody3DComponent(lua_State* L);
int AddRigidBody3DComponent(lua_State* L);

int GetBoxCollider3DComponent(lua_State* L);
int SetBoxCollider3DComponent(lua_State* L);
int AddBoxCollider3DComponent(lua_State* L);

int GetSphereCollider3DComponent(lua_State* L);
int SetSphereCollider3DComponent(lua_State* L);
int AddSphereCollider3DComponent(lua_State* L);

int GetCapsuleCollider3DComponent(lua_State* L);
int SetCapsuleCollider3DComponent(lua_State* L);
int AddCapsuleCollider3DComponent(lua_State* L);

int GetCameraComponent(lua_State* L);
int SetCameraComponent(lua_State* L);
int AddCameraComponent(lua_State* L);

int GetSelectComponent(lua_State* L);
int SetSelectComponent(lua_State* L);
int AddSelectComponent(lua_State* L);

int GetPathComponent(lua_State* L);
int SetPathComponent(lua_State* L);
int AddPathComponent(lua_State* L);

int GetGridComponent(lua_State* L);
int SetGridComponent(lua_State* L);
int AddGridComponent(lua_State* L);

int GetLightComponent(lua_State* L);
int SetLightComponent(lua_State* L);
int AddLightComponent(lua_State* L);

int GetSunComponent(lua_State* L);
int SetSunComponent(lua_State* L);
int AddSunComponent(lua_State* L);

int GetParticleComponent(lua_State* L);
int SetParticleComponent(lua_State* L);
int AddParticleComponent(lua_State* L);

int GetAnimationComponent(lua_State* L);
int SetAnimationComponent(lua_State* L);
int AddAnimationComponent(lua_State* L);


}
}
