/*
 * This header file defines the ImGui layer of Basil.
 */
#pragma once

#include "Core/Layer.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace Basil
{
	class BASIL_API ImGuiLayer : public Layer
	{
		public:
			ImGuiLayer();
			~ImGuiLayer();
			void onAttach() override;
			void onDetach() override;
			void onImGuiRender() override;
			void begin();
			void end();
		private:
			float time;
	};
}
