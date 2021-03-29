/*
 * This header file defines the ImGui layer of Basil.
 */
#pragma once

#include "Application.h"
#include "ImGuiOpenGLRenderer.h"
#include "Layer.h"
#include "GLFW/glfw3.h"

namespace Basil
{
	class BASIL_API ImGuiLayer : public Layer
	{
		public:
			ImGuiLayer();
			~ImGuiLayer();
			void onAttach() override;
			void onDetach() override;
			void onUpdate() override;
			void onEvent(Event& e) override;
		private:
			float time;
	};
}
