#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), cameraController(1280.0f / 720.0f) {}

void Sandbox2D::onAttach()
{
	PROFILE_FUNCTION();

	// Load texture
	texture = Basil::Texture2D::create("assets/textures/test.png");
}

void Sandbox2D::onDetach()
{
	PROFILE_FUNCTION();
}

void Sandbox2D::onUpdate(Basil::Timestep timeStep)
{
	// Start timer for onUpdate
	PROFILE_FUNCTION();

	cameraController.onUpdate(timeStep);

	// Render
	{
		PROFILE_SCOPE("Renderer Prep")
		Basil::Renderer::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Basil::Renderer::clear();
	}

	{
		PROFILE_SCOPE("Renderer Draw");
		Basil::Renderer2D::beginScene(cameraController.getCamera());
		Basil::Renderer2D::drawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, squareColor);
		Basil::Renderer2D::drawQuad({ -1.0f, 0.5f }, { 0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f });
		Basil::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, 10.0f, texture);
		Basil::Renderer::endScene();
	}
}

void Sandbox2D::onEvent(Basil::Event& e)
{
	cameraController.onEvent(e);
}

void Sandbox2D::onImGuiRender()
{
	PROFILE_FUNCTION();

	// Render GUI
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
	ImGui::End();
}