#include "WindowEvent.h"
#include "Types.h"

#include <fmt/core.h>

namespace Sprocket {

WindowResizeEvent::WindowResizeEvent(u32 width, u32 height)
	: d_width(width)
	, d_height(height)
{
}

}