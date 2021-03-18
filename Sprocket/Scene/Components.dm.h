#pragma once
#include "Maths.h"
#include "ECS.h"
#include <queue>
#include <string>

namespace Sprocket{

// Components
#ifdef DATAMATIC_BLOCK
struct {{Comp.Name}}
{
    {{Attr.Type}} {{Attr.Name}} = {{Attr.Default}};
};

#endif

}
