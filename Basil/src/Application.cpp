#include "pch.h"
#include "Application.h"

#include <glad/glad.h>
#include "GLFW/glfw3.h"

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

		// Set renderer API
		Renderer::setAPI(RendererAPI::OpenGL);

		// Set create the window and set the window callback
		window = std::unique_ptr<Window>(Window::create());
		window->setEventCallback(BIND_EVENT(Application::onEvent));

		// Create and push ImGui overlay
		imGuiLayer = new ImGuiLayer();
		pushOverlay(imGuiLayer);

		// Create vertex and index data
		std::vector<float> vertices =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};
		
		std::vector<unsigned int> indices =
		{
			0, 1, 2
		};
	
		// Create VBO and IBO
		vbo.reset(VertexBuffer::create(vertices));
		ibo.reset(IndexBuffer::create(indices));

		// Create VAO and specify format of data
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		vbo->bind();
		ibo->bind();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);

		// Create shaders
		std::string vertexShaderSource = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentShaderSource = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";

		shader.reset(new Shader(vertexShaderSource, fragmentShaderSource));

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
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// Set shader
			shader->bind();

			// Draw triangle
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, ibo->getSize(), GL_UNSIGNED_INT, nullptr);

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