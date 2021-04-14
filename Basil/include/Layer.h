/*
 * This header is for defining an API layer class.
 */
#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Timestep.h"

namespace Basil
{
	class BASIL_API Layer
	{
		public:
			Layer(const std::string& name = "Layer");
			virtual ~Layer() {}
			virtual void onAttach() {}
			virtual void onDetach() {}
			virtual void onUpdate(Timestep timestep) {}
			virtual void onEvent(Event& e) {}
			virtual void onImGuiRender() {}
			inline const std::string& getName() const { return debugName; }
		protected:
			std::string debugName;
	};
}
