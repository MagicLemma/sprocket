#pragma once
#include "Maths.h"
#include <queue>
#include <string>

namespace Sprocket{

// Components
DATAMATIC_BEGIN
struct {{Comp::name}}
{
    {{Attr::type}} {{Attr::name}} = {{Attr::default}};
};

DATAMATIC_END

}
