#pragma once

#include <vector>

#include "Core.h"
#include "Layer.h"

namespace Basil
{
	class BASIL_API LayerStack
	{
		public:
			LayerStack();
			~LayerStack();
			void pushLayer(Layer* layer);
			void pushOverlay(Layer* layer);
			void popLayer(Layer* layer);
			void popOverlay(Layer* layer);
			std::vector<Layer*>::iterator begin();
			std::vector<Layer*>::iterator end();
		private:
			std::vector<Layer*> layers;
			std::vector<Layer*>::iterator layerIterator;
	};
}
