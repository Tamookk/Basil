#include "pch.h"
#include "Application.h"

namespace Basil
{
	// Make Application a singleton
	Application* Application::instance = nullptr;

	// Constructor
	Application::Application()
	{
		// Check we only have one application
		ASSERT(!instance, "Application already exists!");

		// Set application instance
		instance = this;

		// Set create the window and set the window callback
		window = std::unique_ptr<Window>(Window::create());
		window->setEventCallback(BIND_EVENT(Application::onEvent));

		// Create and push ImGui overlay
		imGuiLayer = new ImGuiLayer();
		pushOverlay(imGuiLayer);

		running = true;
	}

	// Destructor
	Application::~Application()
	{

	}

	// Run the application
	void Application::run()
	{
		// Test event and logging systems
		WindowResizeEvent e(1280, 720);
		if (e.isInCategory(EventCategory::ApplicationEvent))
			LOG_TRACE(e);
		if (e.isInCategory(EventCategory::InputEvent))
			LOG_TRACE(e);

		while (running)
		{
			// Do on update stuff
			for (Layer* layer : layerStack)
				layer->onUpdate();

			// Do ImGui stuff
			imGuiLayer->begin();
			for (Layer* layer : layerStack)
				layer->onImGuiRender();
			imGuiLayer->end();

			window->onUpdate();
		}
	}

	// Process an event
	void Application::onEvent(Event& e)
	{
		// Log event
		LOG_TRACE("{0}", e);

		// Dispatch event
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT(Application::onWindowClose));

		for (auto it = layerStack.end(); it != layerStack.begin();)
		{
			(*--it)->onEvent(e);
			if (e.handled)
				break;
		}
	}

	// Push layer to layer stack
	void Application::pushLayer(Layer* layer)
	{
		layerStack.pushLayer(layer);
		layer->onAttach();
	}

	// Push overlay to layer stack
	void Application::pushOverlay(Layer* layer)
	{
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

	// Process window close event
	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		running = false;
		return true;
	}
}