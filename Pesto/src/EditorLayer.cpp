#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

EditorLayer::EditorLayer() : Layer("Sandbox2D"), cameraController(1280.0f / 720.0f) {}

void EditorLayer::onAttach()
{
	PROFILE_FUNCTION();

	// Load texture
	texture = Basil::Texture2D::create("assets/textures/test.png");

	Basil::FramebufferSpecification fbSpec;
	fbSpec.width = 1280;
	fbSpec.height = 720;
	framebuffer = Basil::Framebuffer::create(fbSpec);
}

void EditorLayer::onDetach()
{
	PROFILE_FUNCTION();
}

void EditorLayer::onUpdate(Basil::Timestep timeStep)
{
	// Start timer for onUpdate
	PROFILE_FUNCTION();

	cameraController.onUpdate(timeStep);

	// Render
	Basil::Renderer2D::resetStats();
	{
		PROFILE_SCOPE("Renderer Prep");
		framebuffer->bind();
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
		framebuffer->unbind();
	}
}

void EditorLayer::onEvent(Basil::Event& e)
{
	cameraController.onEvent(e);
}

void EditorLayer::onImGuiRender()
{
	PROFILE_FUNCTION();

	// Switch to true to enable dockspace
	static bool dockingEnabled = true;
	if (dockingEnabled)
	{
		// Setup variables
		static bool dockSpaceOpen = true;
		static bool optFullscreenPersistant = true;
		bool optFullscreen = optFullscreenPersistant;
		static ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None;

		// Set flags
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		// If fullscreen
		if (optFullscreen)
		{
			// Setup fullscreen mode
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// Set some flags
		if (dockSpaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			windowFlags |= ImGuiWindowFlags_NoBackground;

		// Create the ImGui instance
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, windowFlags);
		ImGui::PopStyleVar();

		if (optFullscreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspaceID = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockSpaceFlags);
		}

		// Create menu bar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
					Basil::Application::get().close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// Create settings panel
		ImGui::Begin("Settings");

		Basil::Renderer2D::Statistics stats = Basil::Renderer2D::getStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quadCount);
		ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
		ImGui::Text("Indices: %d", stats.getTotalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));

		uint32_t textureID = framebuffer->getColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ 1280, 720 });

		ImGui::End();
		ImGui::End();
	}
	else
	{
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
}