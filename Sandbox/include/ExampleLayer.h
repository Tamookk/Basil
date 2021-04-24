/*
 * Declares a class with examples of features of Basil.
 */
#pragma once

#include "Core/Basil.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

class ExampleLayer : public Basil::Layer
{
	public:
		ExampleLayer();
		void onUpdate(Basil::Timestep timeStep) override;
		void onEvent(Basil::Event& e) override;
		void onImGuiRender() override;
		bool onKeyPressedEvent(Basil::KeyPressedEvent& event);

	private:
		Basil::ShaderLibrary shaderLibrary;
		Basil::Shared<Basil::Shader> shader;
		Basil::Shared<Basil::VertexArray> vao;

		Basil::Shared<Basil::Shader> squareShader;
		Basil::Shared<Basil::VertexArray> squareVao;

		Basil::Shared<Basil::VertexArray> imageVao;

		Basil::Shared<Basil::Texture2D> texture;

		Basil::OrthographicCameraController cameraController;

		glm::vec3 squareColor = { 0.0f, 0.0f, 0.0f };
};