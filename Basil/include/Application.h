/*
 * This class manages application run-time.
 */
#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"
#include "Input.h"
#include "LayerStack.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Timestep.h"
#include "Window.h"

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
			Unique<Window> window;
			ImGuiLayer* imGuiLayer;
			bool running;
			LayerStack layerStack;
			static Application* instance;
			float lastFrameTime;
	};

	// To be defined in the client
	Application* createApplication();
}
