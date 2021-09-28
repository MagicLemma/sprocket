#include "camera.h"

#include <Sprocket/Graphics/Viewport.h>
#include <Sprocket/Utility/Maths.h>

#include <glm/glm.hpp>

namespace spkt {

glm::mat4 make_view(const glm::vec3& pos, const glm::quat& ori, float pitch)
{
    return glm::inverse(Maths::Transform(
        pos, ori * glm::rotate(glm::identity<glm::quat>(), pitch, {1, 0, 0})
    ));
}

glm::mat4 make_proj(float fov)
{
    return glm::perspective(glm::radians(fov), Viewport::CurrentAspectRatio(), 0.01f, 1000.0f);
}

}