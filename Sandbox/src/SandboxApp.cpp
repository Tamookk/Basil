/*
 * This cpp file is used to test the Basil Game Engine as it is being developed. 
 */

#include "Basil.h"
#include "imgui.h"

class ExampleLayer : public Basil::Layer
{
	public:
		ExampleLayer() : Layer("Example"), camera(-1.6f, 1.6f, -0.9f, 0.9f), cameraPosition(0.0f)
		{
			// Initialise variables
			cameraRotation = 0.0f;
			cameraRotateSpeed = 1.0f;
			cameraXSpeed = 0.1f;
			cameraYSpeed = 0.1f;

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
			std::shared_ptr<Basil::VertexBuffer> vbo;
			vbo.reset(Basil::VertexBuffer::create(vertices));

			// Set VBO's layout
			{
				Basil::BufferLayout layout =
				{
					{ "a_Position", ShaderDataType::Float3 },
					{ "a_Color",	ShaderDataType::Float4 }
				};

				vbo->setLayout(layout);
			}

			// Create IBO
			std::shared_ptr<Basil::IndexBuffer> ibo;
			ibo.reset(Basil::IndexBuffer::create(indices));

			// Create VAO and specify format of data
			vao.reset(Basil::VertexArray::create());
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

				uniform mat4 u_ViewProjection;
			
				out vec3 v_Position;
				out vec4 v_Color;

				void main()
				{
					v_Position = a_Position;
					v_Color = a_Color;
					gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
				}
			)";

			std::string fragmentShaderSource = R"(
				#version 330 core
			
				layout(location = 0) out vec4 color;

				in vec4 v_Color;
			
				void main()
				{
					color = v_Color;
				}
			)";

			shader.reset(new Basil::Shader(vertexShaderSource, fragmentShaderSource));
		}

		void onUpdate() override
		{
			// Poll for input
			if (Basil::Input::isKeyPressed(BASIL_KEY_RIGHT))
				cameraPosition.x += cameraXSpeed;
			else if (Basil::Input::isKeyPressed(BASIL_KEY_LEFT))
				cameraPosition.x -= cameraXSpeed;
			
			if (Basil::Input::isKeyPressed(BASIL_KEY_UP))
				cameraPosition.y += cameraYSpeed;
			else if (Basil::Input::isKeyPressed(BASIL_KEY_DOWN))
				cameraPosition.y -= cameraYSpeed;

			if (Basil::Input::isKeyPressed(BASIL_KEY_A))
				cameraRotation += cameraRotateSpeed;
			else if (Basil::Input::isKeyPressed(BASIL_KEY_D))
				cameraRotation -= cameraRotateSpeed;

			// Set the clear colour and clear the screen
			Basil::Renderer::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			Basil::Renderer::clear();

			// Set the camera's position and rotation
			camera.setPosition(cameraPosition);
			camera.setRotation(cameraRotation);

			// Begin the scene
			Basil::Renderer::beginScene(camera);

			// Set shader
			shader->bind();

			// Draw triangle
			Basil::Renderer::submit(shader, vao);

			// End scene
			Basil::Renderer::endScene();
		}

		void onEvent(Basil::Event& e) override
		{
			Basil::EventDispatcher dispatcher(e);
			dispatcher.dispatch<Basil::KeyPressedEvent>(BIND_EVENT(ExampleLayer::onKeyPressedEvent));
		}

		void onImGuiRender() override {}

		bool onKeyPressedEvent(Basil::KeyPressedEvent& event)
		{
			return false;
		}

	private:
		std::shared_ptr<Basil::Shader> shader;
		std::shared_ptr<Basil::VertexArray> vao;
		Basil::OrthographicCamera camera;
		glm::vec3 cameraPosition;
		float cameraRotation;
		float cameraXSpeed;
		float cameraYSpeed;
		float cameraRotateSpeed;
};

class Sandbox : public Basil::Application
{
	public:
		Sandbox()
		{
			pushLayer(new ExampleLayer());
		}

		~Sandbox()
		{

		}
};

// Create a new instance of Sandbox
Basil::Application* Basil::createApplication()
{
	return new Sandbox();
}