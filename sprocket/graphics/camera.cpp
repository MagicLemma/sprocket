#include "camera.h"

#include <sprocket/graphics/Viewport.h>
#include <sprocket/Utility/Maths.h>

#include <glm/glm.hpp>

namespace spkt {

glm::mat4 make_view(const glm::vec3& pos, const glm::quat& ori, float pitch)
{
    return glm::inverse(make_transform(
        pos, ori * glm::rotate(glm::identity<glm::quat>(), pitch, {1, 0, 0})
    ));
}

glm::mat4 make_proj(float fov)
{
    return glm::perspective(glm::radians(fov), viewport::current_aspect_ratio(), 0.01f, 1000.0f);
}

}