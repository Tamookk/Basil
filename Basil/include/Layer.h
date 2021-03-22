#pragma once

#include "Core.h"
#include "Event.h"

namespace Basil
{
	class BASIL_API Layer
	{
		public:
			Layer(const std::string& name = "Layer");
			virtual ~Layer() {}
			virtual void onAttach() {}
			virtual void onDetach() {}
			virtual void onUpdate() {}
			virtual void onEvent(Event& e) {}
			inline const std::string& getName() const { return debugName; }
		protected:
			std::string debugName;
	};
}
