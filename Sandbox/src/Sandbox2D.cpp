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
	Basil::Renderer2D::resetStats();
	{
		PROFILE_SCOPE("Renderer Prep");
		Basil::Renderer::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Basil::Renderer::clear();
	}

	{
		PROFILE_SCOPE("Renderer Draw");
		Basil::Renderer2D::beginScene(cameraController.getCamera());

		Basil::Transform transform;
		Basil::Renderer2D::drawQuad(transform, squareColor);

		transform.position = { -1.0f, 0.5f, 0.0f };
		transform.scale = { 0.5f, 0.5f, 1.0f };
		Basil::Renderer2D::drawQuad(transform, { 1.0f, 0.0f, 0.0f, 1.0f });

		transform.position = { 0.0f, 0.0f, -0.1f };
		transform.rotation = 45.0f;
		transform.scale = { 10.0f, 10.0f, 1.0f };
		Basil::Renderer2D::drawQuad(transform, texture, 10.0f);

		Basil::Renderer2D::endScene();

		Basil::Renderer2D::beginScene(cameraController.getCamera());
		transform.rotation = 0.0f;
		transform.scale = { 1.0f, 1.0f, 1.0f };
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				transform.position = { x, y, 0.0f };
				Basil::Renderer2D::drawQuad(transform, color);
			}
		}
		Basil::Renderer2D::endScene();
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

	Basil::Renderer2D::Statistics stats = Basil::Renderer2D::getStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.drawCalls);
	ImGui::Text("Quads: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
	ImGui::Text("Indices: %d", stats.getTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
	ImGui::End();
}