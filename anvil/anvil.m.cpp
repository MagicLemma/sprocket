#include <anvil/anvil.h>

#include <sprocket/core.h>

auto main() -> int
{
    return spkt::run_app_framerate<anvil::app>("Anvil");
}
