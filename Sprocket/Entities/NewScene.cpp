#include "NewScene.h"

namespace Sprocket {
    
NewScene::NewScene()
    : d_registry()
{    
}

entt::entity NewScene::CreateEntity()
{
    return d_registry.create();
}

void NewScene::DestroyEntity(entt::entity entity)
{
    d_registry.destroy(entity);
}

}