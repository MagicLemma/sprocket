#include "Core/Initialiser.h"
#include "Utility/Log.h"
#include "Graphics/Loader.h"

#include <GLFW/glfw3.h>

namespace Sprocket {

Initialiser::Initialiser()
{
    Sprocket::Log::init();
    SPKT_LOG_INFO("Version {}.{}.{}", 0, 0, 1);

    // Initialise GLFW
	glfwInit();

    // Initialise Loader
    Loader::init();
}

Initialiser::~Initialiser()
{
    Loader::deinit();

    glfwTerminate();
}    

}