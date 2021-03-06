/*
 * This header file defines the ImGui layer of Basil.
 */
#pragma once

#include "Core/Layer.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace Basil
{
	class ImGuiLayer : public Layer
	{
		public:
			ImGuiLayer();
			~ImGuiLayer() {}
			void onAttach() override;
			void onDetach() override;
			void onEvent(Event& e) override;
			void onImGuiRender() override;
			void begin();
			void end();
			void setBlockEvents(bool block);
			void setDarkThemeColors();
		private:
			bool blockEvents;
	};
}
