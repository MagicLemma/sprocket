#include "Entry.h"
#include "Application.h"
#include "Log.h"

#include <iostream>

#include <spdlog/spdlog.h>

namespace Sprocket {

int begin(int argc, char** argv, Application* app)
{
    Sprocket::Log::init();
    SPKT_LOG_CORE_INFO("Version {}.{}.{}", 0, 0, 1);
    app->run();
    return 0;
}

}