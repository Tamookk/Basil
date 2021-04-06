/*
 * This class manages application run-time
 */
#pragma once

#include "ApplicationEvent.h"
#include "Core.h"
#include "Event.h"
#include "ImGuiLayer.h"
#include "IndexBuffer.h"
#include "Input.h"
#include "LayerStack.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
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
			std::unique_ptr<Window> window;
			ImGuiLayer* imGuiLayer;
			bool running;
			LayerStack layerStack;
			static Application* instance;
			std::shared_ptr<Shader> shader;
			std::shared_ptr<VertexArray> vao;
	};

	// To be defined in the client
	Application* createApplication();
}
