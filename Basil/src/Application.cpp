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

		// Create vertex and index data
		std::vector<float> vertices =
		{
			-0.5f, -0.5f, 0.0f,			// vertex 0 xyz
			 1.0f,  0.0f, 0.0f, 1.0f,	// vertex 0 rgba
			 0.5f, -0.5f, 0.0f,			// vertex 1 xyz
			 0.0f,  1.0f, 0.0f, 1.0f,	// vertex 1 rgba
			 0.0f,  0.5f, 0.0f,			// vertex 2 xyz
			 0.0f,  0.0f, 1.0f, 1.0f	// vertex 2 rgba
		};
		
		std::vector<unsigned int> indices =
		{
			0, 1, 2
		};
	
		// Create VBO
		std::shared_ptr<VertexBuffer> vbo;
		vbo.reset(VertexBuffer::create(vertices));
		
		// Set VBO's layout
		{
			BufferLayout layout =
			{
				{ "a_Position", ShaderDataType::Float3 },
				{ "a_Color",	ShaderDataType::Float4 }
			};

			vbo->setLayout(layout);
		}

		// Create IBO
		std::shared_ptr<IndexBuffer> ibo;
		ibo.reset(IndexBuffer::create(indices));

		// Create VAO and specify format of data
		vao.reset(VertexArray::create());
		vao->bind();
		vao->addVertexBuffer(vbo);
		vao->setIndexBuffer(ibo);

		vbo->bind();
		ibo->bind();

		// Create shaders
		std::string vertexShaderSource = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentShaderSource = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;
			in vec4 v_Color;
			
			void main()
			{
				color = v_Color;
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
			Renderer::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			Renderer::clear();
			Renderer::beginScene();

			// Set shader
			shader->bind();

			// Draw triangle
			Renderer::submit(vao);

			Renderer::endScene();

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