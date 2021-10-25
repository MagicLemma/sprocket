#include <anvil/runtime.h>

#include <sprocket/core.h>

int main()
{
    return spkt::run_app_framerate<anvil::runtime>("Runtime");
}