#include "pch.h"
#include "LayerStack.h"

namespace Basil
{
	// Constructor
	LayerStack::LayerStack()
	{
		layerIterator = layers.begin();
	}

	// Destructor
	LayerStack::~LayerStack()
	{
		for (Layer* layer : layers)
			delete layer;
	}

	// Push a layer to the layers vector
	void LayerStack::pushLayer(Layer * layer)
	{
		layerIterator = layers.emplace(layerIterator, layer);
	}

	// Push an overlay to the layers vector (back of vector)
	void LayerStack::pushOverlay(Layer* layer)
	{
		layers.emplace_back(layer);
	}

	// Pop a layer from the layers bector
	void LayerStack::popLayer(Layer* layer)
	{
		// Find the layer in layers
		auto it = std::find(layers.begin(), layers.end(), layer);
		
		// Remove the layer if it is in the vector
		if (it != layers.end())
		{
			layers.erase(it);
			layerIterator--;
		}
	}

	// Pop a layer overlay from layers vector
	void LayerStack::popOverlay(Layer* layer)
	{
		auto it = std::find(layers.begin(), layers.end(), layer);
		if (it != layers.end())
			layers.erase(it);
	}

	// Start of layers vector
	std::vector<Layer*>::iterator LayerStack::begin() { return layers.begin(); }

	// End of layers vector
	std::vector<Layer*>::iterator LayerStack::end() { return layers.end(); }
}