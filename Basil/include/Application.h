/*
 * This class manages application run-time
 */
#pragma once

#include "ApplicationEvent.h"
#include "Core.h"
#include "Event.h"
#include "Window.h"
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
		private:
			bool onWindowClose(WindowCloseEvent& e);
			std::unique_ptr<Window> window;
			bool running;
	};

	// To be defined in the client
	Application* createApplication();
}
