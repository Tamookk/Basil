#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

// Constructor
EditorLayer::EditorLayer() : Layer("Sandbox2D"), cameraController(1280.0f / 720.0f)
{
	viewportFocused = false;
	viewportHovered = false;
}

void EditorLayer::onAttach()
{
	PROFILE_FUNCTION();

	// Load texture
	texture = Basil::Texture2D::create("assets/textures/test.png");

	// Create framebuffer
	Basil::FramebufferSpecification fbSpec;
	fbSpec.width = 1280;
	fbSpec.height = 720;
	framebuffer = Basil::Framebuffer::create(fbSpec);

	// Create scene
	activeScene = Basil::makeShared<Basil::Scene>();

	// Create square entity
	auto square = activeScene->createEntity("Green Square");
	square.addComponent<Basil::SpriteRenderComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
	squareEntity = square;
}

void EditorLayer::onDetach()
{
	PROFILE_FUNCTION();
}

void EditorLayer::onUpdate(Basil::Timestep timeStep)
{
	// Start timer for onUpdate
	PROFILE_FUNCTION();

	// Stop flickering on resize
	{
		Basil::FramebufferSpecification spec = framebuffer->getSpecification();
		if (viewportSize.x > 0.0f && viewportSize.y > 0.0f && (spec.width != viewportSize.x || spec.height != viewportSize.y))
		{
			framebuffer->resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			cameraController.resize(viewportSize.x, viewportSize.y);
		}
	}

	// Update camera if viewport is focused
	if(viewportFocused)
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
		activeScene->onUpdate(timeStep);

		Basil::TransformComponent transform;
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


	// Menu bar
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


	// Settings panel
	ImGui::Begin("Settings");
	Basil::Renderer2D::Statistics stats = Basil::Renderer2D::getStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.drawCalls);
	ImGui::Text("Quads: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
	ImGui::Text("Indices: %d", stats.getTotalIndexCount());
	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));

	if (squareEntity)
	{
		ImGui::Separator();
		auto& tag = squareEntity.getComponent<Basil::TagComponent>().tag;
		ImGui::Text("%s", tag.c_str());
		auto& greenSquareColor = squareEntity.getComponent<Basil::SpriteRenderComponent>().color;
		ImGui::ColorEdit4("Green Square Color", glm::value_ptr(greenSquareColor));
		ImGui::Separator();
	}

	ImGui::End();


	// Viewport
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	ImGui::Begin("Viewport");
	viewportFocused = ImGui::IsWindowFocused();
	viewportHovered = ImGui::IsWindowHovered();
	Basil::Application::get().getImGuiLayer()->setBlockEvents(!viewportFocused || !viewportHovered);
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
	uint32_t textureID = framebuffer->getColorAttachmentRendererID();
	ImGui::Image((void*)textureID, viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();
	ImGui::PopStyleVar();


	ImGui::End();
}