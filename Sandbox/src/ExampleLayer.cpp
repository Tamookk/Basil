#include "ExampleLayer.h"
/*
// Constructor
ExampleLayer::ExampleLayer() : Layer("Example"), cameraController(1280.0f / 720.0f)
{
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

	std::vector<float> squareVertices =
	{
		-0.6f, -0.6f, 0.0f,			// vertex 0 xyz
		 //1.0f,  1.0f, 1.0f, 1.0f,	// vertex 0 rgba
		-0.6f,  0.6f, 0.0f,			// vertex 1 xyz
		 //1.0f,  1.0f, 1.0f, 1.0f,	// vertex 1 rgba
		 0.6f, -0.6f, 0.0f,			// vertex 2 xyz
		// 1.0f,  1.0f, 1.0f, 1.0f,	// vertex 2 rgba
		 0.6f,  0.6f, 0.0f,			// vertex 3 xyz
		 //1.0f,  1.0f, 1.0f, 1.0f	// vertex 3 rgba
	};

	std::vector<float> imageVertices =
	{
		-0.5f, -0.5f, 0.0f,
		 0.0f,  0.0f,
		-0.5f,  0.5f, 0.0f,
		 0.0f,  1.0f,
		 0.5f, -0.5f, 0.0f,
		 1.0f,  0.0f,
		 0.5f,  0.5f, 0.0f,
		 1.0f,  1.0f
	};

	std::vector<unsigned int> indices =
	{
		0, 1, 2
	};

	std::vector<unsigned int> squareIndices =
	{
		0, 1, 2,
		2, 3, 1
	};

	std::vector<unsigned int> imageIndices =
	{
		0, 1, 2,
		2, 3, 1
	};

	// Create VBO and set layout
	Basil::Shared<Basil::VertexBuffer> vbo = Basil::VertexBuffer::create(vertices);
	{
		Basil::BufferLayout layout =
		{
			{ "a_Position", ShaderDataType::Float3 },
			{ "a_Color",	ShaderDataType::Float4 }
		};

		vbo->setLayout(layout);
	}

	Basil::Shared<Basil::VertexBuffer> squareVbo = Basil::VertexBuffer::create(squareVertices);
	{
		Basil::BufferLayout layout =
		{
			{ "a_Position", ShaderDataType::Float3 },
			//{ "a_Color",	ShaderDataType::Float4 }
		};

		squareVbo->setLayout(layout);
	}

	Basil::Shared<Basil::VertexBuffer> imageVbo = Basil::VertexBuffer::create(imageVertices);
	{
		Basil::BufferLayout layout =
		{
			{ "a_Position", ShaderDataType::Float3 },
			{ "a_TexCoord",	ShaderDataType::Float2 }
		};

		imageVbo->setLayout(layout);
	}

	// Create IBO
	Basil::Shared<Basil::IndexBuffer> ibo = Basil::IndexBuffer::create(indices);

	Basil::Shared<Basil::IndexBuffer> squareIbo = Basil::IndexBuffer::create(squareIndices);

	Basil::Shared<Basil::IndexBuffer> imageIbo = Basil::IndexBuffer::create(imageIndices);

	// Create VAO and specify format of data
	vao = Basil::VertexArray::create();
	vao->addVertexBuffer(vbo);
	vao->setIndexBuffer(ibo);

	squareVao = Basil::VertexArray::create();
	squareVao->addVertexBuffer(squareVbo);
	squareVao->setIndexBuffer(squareIbo);

	imageVao = Basil::VertexArray::create();
	imageVao->addVertexBuffer(imageVbo);
	imageVao->setIndexBuffer(imageIbo);

	// Create shaders
	std::string vertexShaderSource = R"(
				#version 330 core
			
				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec4 a_Color;

				uniform mat4 u_ViewProjection;
				uniform mat4 u_Transform;
			
				out vec3 v_Position;
				out vec4 v_Color;

				void main()
				{
					v_Position = a_Position;
					v_Color = a_Color;
					gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
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

	std::string vertexShaderSourceSquare = R"(
				#version 330 core
			
				layout(location = 0) in vec3 a_Position;

				uniform mat4 u_ViewProjection;
				uniform mat4 u_Transform;
			
				out vec3 v_Position;
				out vec4 v_Color;

				void main()
				{
					v_Position = a_Position;
					gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
				}
			)";

	std::string fragmentShaderSourceSquare = R"(
				#version 330 core
			
				layout(location = 0) out vec4 color;

				uniform vec3 u_Color;
			
				void main()
				{
					color = vec4(u_Color, 1.0);
				}
			)";

	shader = Basil::Shader::create("VertexPosColor", vertexShaderSource, fragmentShaderSource);
	squareShader = Basil::Shader::create("SquareShader", vertexShaderSourceSquare, fragmentShaderSourceSquare);
	auto imageShader = shaderLibrary.load("assets/shaders/Texture.glsl");

	// Load texture and upload to uniform
	texture = Basil::Texture2D::create("assets/textures/test.png");
	imageShader->bind();
	imageShader->setInt("u_Texture", 0);
}

// On update
void ExampleLayer::onUpdate(Basil::Timestep timeStep)
{
	// Update camera
	cameraController.onUpdate(timeStep);

	// Set the clear colour and clear the screen
	Basil::Renderer::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Basil::Renderer::clear();

	// Begin the scene
	Basil::Renderer::beginScene(cameraController.getCamera());

	// Draw square
	squareShader->bind();
	squareShader->setFloat3("u_Color", squareColor);
	Basil::Renderer::submit(squareShader, squareVao);

	// Draw triangles
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			Basil::Renderer::submit(shader, vao, transform);
		}
	}

	// Draw square with texture
	auto imageShader = shaderLibrary.get("Texture");
	texture->bind();
	Basil::Renderer::submit(imageShader, imageVao, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	// End scene
	Basil::Renderer::endScene();
}

// On event
void ExampleLayer::onEvent(Basil::Event& e)
{
	cameraController.onEvent(e);
}

// on ImGui Render
void ExampleLayer::onImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(squareColor));
	ImGui::End();
}

// On Key Pressed Event
bool ExampleLayer::onKeyPressedEvent(Basil::KeyPressedEvent& event)
{
	return false;
}*/