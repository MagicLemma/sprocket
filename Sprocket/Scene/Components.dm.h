#pragma once
#include "Maths.h"
#include <queue>
#include <string>

namespace Sprocket{

// Components
DATAMATIC_BLOCK_START
struct {{Comp::name}}
{
    {{Attr::type}} {{Attr::name}} = {{Attr::default}};
};

DATAMATIC_BLOCK_END

}
