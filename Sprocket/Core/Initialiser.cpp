#include "Core/Initialiser.h"
#include "Utility/Log.h"

#include <GLFW/glfw3.h>

namespace Sprocket {

Initialiser::Initialiser()
{
    Sprocket::Log::init();
    SPKT_LOG_INFO("Version {}.{}.{}", 0, 0, 1);

    // Initialise GLFW
	glfwInit();
}

Initialiser::~Initialiser()
{
    glfwTerminate();
}    

}