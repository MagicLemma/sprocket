#include "LayerStack.h"

#include <algorithm>

namespace Sprocket {

LayerStack::LayerStack()
	: d_layers()
	, d_layerLevel(0)
{
}

void LayerStack::pushLayer(LayerPtr layer)
{
	d_layers.emplace(d_layers.begin() + d_layerLevel, layer);
	++d_layerLevel;
}

void LayerStack::pushOverlay(LayerPtr layer)
{
	d_layers.emplace_back(layer);
}

bool LayerStack::popLayer(LayerPtr layer)
{
	auto it = std::find(d_layers.begin(), d_layers.begin() + d_layerLevel, layer);
	if (it != d_layers.begin() + d_layerLevel) {
		layer->onDetach();
		d_layers.erase(it);
		--d_layerLevel;
		return true;
	}
	return false;
}

bool LayerStack::popOverlay(LayerPtr layer)
{
	auto it = std::find(d_layers.begin() + d_layerLevel, d_layers.end(), layer);
	if (it != d_layers.begin() + d_layerLevel) {
		layer->onDetach();
		d_layers.erase(it);
		return true;
	}
	return false;
}

}