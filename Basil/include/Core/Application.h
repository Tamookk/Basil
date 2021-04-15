/*
 * This class manages application run-time.
 */
#pragma once

#include "Core/Core.h"
#include "Core/Input.h"
#include "Core/LayerStack.h"
#include "Core/Timestep.h"
#include "Core/Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"

namespace Basil
{
	class BASIL_API Application
	{
		public:
			Application();
			virtual ~Application();
			void run();
			void onEvent(Event& e);
			void pushLayer(Layer* layer);
			void pushOverlay(Layer* layer);
			static Application& get();
			Window& getWindow();
		private:
			bool onWindowClose(WindowCloseEvent& e);
			bool onWindowResize(WindowResizeEvent& e);
			Unique<Window> window;
			ImGuiLayer* imGuiLayer;
			bool running;
			bool minimised;
			LayerStack layerStack;
			static Application* instance;
			float lastFrameTime;
	};

	// To be defined in the client
	Application* createApplication();
}
