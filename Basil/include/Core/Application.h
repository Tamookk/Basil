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

int main(int argc, char** argv);

namespace Basil
{
	class Application
	{
		public:
			Application();
			virtual ~Application();
			void close();
			void onEvent(Event& e);
			void pushLayer(Layer* layer);
			void pushOverlay(Layer* layer);
			static Application& get();
			Window& getWindow();
		private:
			void run();
			bool onWindowClose(WindowCloseEvent& e);
			bool onWindowResize(WindowResizeEvent& e);
			Unique<Window> window;
			ImGuiLayer* imGuiLayer;
			bool running;
			bool minimised;
			LayerStack layerStack;
			static Application* instance;
			float lastFrameTime;
			friend int ::main(int argc, char** argv);
	};

	// To be defined in the client
	Application* createApplication();
}
