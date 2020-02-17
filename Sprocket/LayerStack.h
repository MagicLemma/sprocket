#pragma once
#include "Layer.h"

#include <vector>
#include <memory>

namespace Sprocket {

using LayerPtr = std::shared_ptr<Layer>;
using LayerVec = std::vector<LayerPtr>;

class LayerStack
{
	LayerVec d_layers;
	size_t   d_layerLevel;
	
public:
	LayerStack();

	void pushLayer(LayerPtr layer);
	void pushOverlay(LayerPtr layer);

	bool popLayer(LayerPtr layer);
	bool popOverlay(LayerPtr layer);

	LayerVec::iterator begin() { return d_layers.begin(); }
	LayerVec::iterator end() { return d_layers.end(); }

	LayerVec::reverse_iterator rbegin() { return d_layers.rbegin(); }
	LayerVec::reverse_iterator rend() { return d_layers.rend(); }

	LayerVec::const_iterator cbegin() const { return d_layers.cbegin(); }
	LayerVec::const_iterator cend() const { return d_layers.cend(); }

	LayerVec::const_reverse_iterator crbegin() const { return d_layers.crbegin(); }
	LayerVec::const_reverse_iterator crend() const { return d_layers.crend(); }
};

}