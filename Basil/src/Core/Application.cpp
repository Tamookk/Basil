#include "pch.h"
#include "Core/Application.h"

#include <GLFW/glfw3.h>	// temporary

namespace Basil
{
	// Make Application a singleton
	Application* Application::instance = nullptr;

	// Constructor
	Application::Application()
	{
		PROFILE_FUNCTION();

		// Check we only have one application
		ASSERT(!instance, "Application already exists!");

		// Initialise variables
		running = true;
		minimised = false;
		lastFrameTime = 0.0f;

		// Set application instance
		instance = this;

		// Set create the window and set the window callback
		window = Window::create();
		window->setEventCallback(BIND_EVENT(Application::onEvent));

		// Initialise the renderer
		Renderer::init();

		// Create and push ImGui overlay
		imGuiLayer = new ImGuiLayer();
		pushOverlay(imGuiLayer);
	}

	// Destructor
	Application::~Application()
	{
		PROFILE_FUNCTION();

		Renderer::shutdown();
	}

	// Close application
	void Application::close()
	{
		running = false;
	}

	// Process an event
	void Application::onEvent(Event& e)
	{
		PROFILE_FUNCTION();

		// Dispatch event
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT(Application::onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT(Application::onWindowResize));

		for (auto it = layerStack.end(); it != layerStack.begin();)
		{
			if (e.handled)
				break;
			(*--it)->onEvent(e);
		}
	}

	// Push layer to layer stack
	void Application::pushLayer(Layer* layer)
	{
		PROFILE_FUNCTION();

		layerStack.pushLayer(layer);
		layer->onAttach();
	}

	// Push overlay to layer stack
	void Application::pushOverlay(Layer* layer)
	{
		PROFILE_FUNCTION();

		layerStack.pushOverlay(layer);
		layer->onAttach();
	}

	// Return the application instance
	Application& Application::get()
	{
		return *instance;
	}

	// Return the window pointer
	Window& Application::getWindow()
	{
		return *window;
	}

	// Run the application
	void Application::run()
	{
		PROFILE_FUNCTION();

		while (running)
		{
			PROFILE_SCOPE("RunLoop");

			// Update last frame time
			float time = (float)glfwGetTime();
			Timestep timeStep = time - lastFrameTime;
			lastFrameTime = time;

			// Do on update stuff for each layer if the window is not minimised
			if (!minimised)
			{
				{
					PROFILE_SCOPE("LayerStack onUpdate");
					for (Layer* layer : layerStack)
						layer->onUpdate(timeStep);
				}
			
				// Do ImGui stuff
				imGuiLayer->begin();
				{
					PROFILE_SCOPE("LayerStack onImGuiRender");
					for (Layer* layer : layerStack)
						layer->onImGuiRender();
				}
				imGuiLayer->end();
			}
			// Call window's on update function
			window->onUpdate();
		}
	}

	// Process window close event
	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		running = false;
		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e)
	{
		PROFILE_FUNCTION();

		if (e.getWidth() == 0 || e.getHeight() == 0)
		{
			minimised = true;
			return false;
		}

		minimised = false;
		Renderer::onWindowResize(e.getWidth(), e.getHeight());

		return false;
	}
}