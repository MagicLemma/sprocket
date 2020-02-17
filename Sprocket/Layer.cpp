#include "Layer.h"

#include <string>

namespace Sprocket {

Layer::Layer(const std::string & name)
	: d_name(name)
{
}

Layer::~Layer()
{
}

}