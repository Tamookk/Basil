/*
 * This class manages application run-time
 */
#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Event.h"
#include "ApplicationEvent.h"
#include "GLFW/glfw3.h"

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
		private:
			bool onWindowClose(WindowCloseEvent& e);
			std::unique_ptr<Window> window;
			bool running;
			LayerStack layerStack;
	};

	// To be defined in the client
	Application* createApplication();
}
