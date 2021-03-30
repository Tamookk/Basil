/*
 * This header file defines the ImGui layer of Basil.
 */
#pragma once

#include "Application.h"
#include "ImGuiOpenGLRenderer.h"
#include "KeyEvent.h"
#include "Layer.h"
#include "MouseEvent.h"
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
			bool onKeyPressedEvent(KeyPressedEvent& e);
			bool onKeyReleasedEvent(KeyReleasedEvent& e);
			bool onKeyTypedEvent(KeyTypedEvent& e);
			bool onMouseButtonPressedEvent(MouseButtonPressedEvent& e);
			bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
			bool onMouseMovedEvent(MouseMovedEvent& e);
			bool onMouseScolledEvent(MouseScrolledEvent& e);
			bool onWindowResizeEvent(WindowResizeEvent& e);
			float time;
	};
}
