#include "pch.h"
#include "Application.h"

namespace Basil
{

	// Constructor
	Application::Application()
	{
		window = std::unique_ptr<Window>(Window::create());
		window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));
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
			glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
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
		dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onWindowClose, this, std::placeholders::_1));
	}

	// Process window close event
	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		running = false;
		return true;
	}
}