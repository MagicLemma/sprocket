#pragma once
#include "Maths.h"
#include <queue>
#include <string>

namespace Sprocket{

// Components
#ifdef DATAMATIC_BLOCK
struct {{Comp.name}}
{
    {{Attr.type}} {{Attr.name}} = {{Attr.default}};
};

#endif

}
