#include "library.h"

#include <spdlog/spdlog.h>

int foosticles(int blah)
{
    spdlog::warn("Library log {}", blah);

    return blah * blah;
}