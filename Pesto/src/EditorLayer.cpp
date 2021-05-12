#include "EditorLayer.h"
#include "Scene/SceneSerializer.h"
#include "Utils/PlatfomUtils.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Basil
{
	// Constructor
	EditorLayer::EditorLayer() : Layer("Sandbox2D")
	{
		viewportFocused = false;
		viewportHovered = false;
	}

	void EditorLayer::onAttach()
	{
		PROFILE_FUNCTION();

		// Create framebuffer
		FramebufferSpecification fbSpec;
		fbSpec.width = 1280;
		fbSpec.height = 720;
		framebuffer = Framebuffer::create(fbSpec);

		// Create scene
		activeScene = makeShared<Scene>();

		// Create square entities
		auto square = activeScene->createEntity("Green Square");
		square.addComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
		squareEntity = square;

		auto redSquare = activeScene->createEntity("Red Square");
		redSquare.addComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
		redSquare.getComponent<TransformComponent>().translation = { 0.0f, 1.0f, 0.0f };

		cameraEntity = activeScene->createEntity("Camera");
		cameraEntity.addComponent<CameraComponent>();

		secondCameraEntity = activeScene->createEntity("Clip-Space Entity");
		auto& cc = secondCameraEntity.addComponent<CameraComponent>();
		cc.primary = false;

		class CameraController : public ScriptableEntity
		{
			public:
				void onCreate() override {}

				void onDestroy() override {}

				void onUpdate(Timestep timeStep) override
				{
					auto& translation = getComponent<TransformComponent>().translation;
					float speed = 5.0f;

					if (Input::isKeyPressed(KeyCode::A))
						translation.x -= speed * timeStep;
					if (Input::isKeyPressed(KeyCode::D))
						translation.x += speed * timeStep;
					if (Input::isKeyPressed(KeyCode::W))
						translation.y += speed * timeStep;
					if (Input::isKeyPressed(KeyCode::S))
						translation.y -= speed * timeStep;
				}
		};

		cameraEntity.addComponent<NativeScriptComponent>().bind<CameraController>();

		propertiesPanel.setContext(activeScene);
		sceneHierarchyPanel.setContext(activeScene);
	}

	void EditorLayer::onDetach()
	{
		PROFILE_FUNCTION();
	}

	void EditorLayer::onUpdate(Timestep timeStep)
	{
		// Start timer for onUpdate
		PROFILE_FUNCTION();

		// Stop flickering on resize
		{
			FramebufferSpecification spec = framebuffer->getSpecification();
			if (viewportSize.x > 0.0f && viewportSize.y > 0.0f && (spec.width != viewportSize.x || spec.height != viewportSize.y))
			{
				framebuffer->resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
				activeScene->onViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			}
		}

		// Render
		Renderer2D::resetStats();
		{
			PROFILE_SCOPE("Renderer Prep");
			framebuffer->bind();
			Renderer::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			Renderer::clear();
		}

		{
			PROFILE_SCOPE("Renderer Draw");
			activeScene->onUpdate(timeStep);
			framebuffer->unbind();
		}
	}

	void EditorLayer::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT(EditorLayer::onKeyPressed));
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
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspaceID = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockSpaceFlags);
		}
		style.WindowMinSize.x = minWinSizeX;

		// Menu bar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// New scene
				if (ImGui::MenuItem("New", "Ctrl+N"))
					newScene();

				// Open scene file
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					openScene();

				// Save scene file
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					saveSceneAs();

				// Exit application
				if (ImGui::MenuItem("Exit"))
					Application::get().close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// Scene hierarchy panel
		sceneHierarchyPanel.onImGuiRender();

		// Properties panel
		propertiesPanel.onImGuiRender(sceneHierarchyPanel.getSelectionContext());

		// Settings panel
		ImGui::Begin("Stats");
		Renderer2D::Statistics stats = Renderer2D::getStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quadCount);
		ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
		ImGui::Text("Indices: %d", stats.getTotalIndexCount());
		ImGui::End();


		// Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Viewport");
		viewportFocused = ImGui::IsWindowFocused();
		viewportHovered = ImGui::IsWindowHovered();
		Application::get().getImGuiLayer()->setBlockEvents(!viewportFocused || !viewportHovered);
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		uint64_t textureID = framebuffer->getColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();


		ImGui::End();
	}

	// On key pressed function
	bool EditorLayer::onKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.getRepeatCount() > 0)
			return false;

		bool control = Input::isKeyPressed(Key::LeftControl) || Input::isKeyPressed(Key::RightControl);
		bool shift = Input::isKeyPressed(Key::LeftShift) || Input::isKeyPressed(Key::RightShift);
		switch (e.getKeycode())
		{
			case (int)Key::N:
				if (control)
					newScene();
				break;
			case (int)Key::O:
				if (control)
					openScene();
				break;
			case (int)Key::S:
				if (control && shift)
					saveSceneAs();
				break;
		}
		return false;
	}

	// Make a new scene
	void EditorLayer::newScene()
	{
		activeScene = makeShared<Scene>();
		activeScene->onViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		sceneHierarchyPanel.setContext(activeScene);
	}

	// Open a scene
	void EditorLayer::openScene()
	{
		std::string filePath = FileDialogs::openFile("Basil Scene (*.scene)\0*.scene\0");
		if (!filePath.empty())
		{
			activeScene = makeShared<Scene>();
			activeScene->onViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			sceneHierarchyPanel.setContext(activeScene);

			SceneSerializer serializer(activeScene);
			serializer.deserialize(filePath);
		}
	}

	// Save a scene as
	void EditorLayer::saveSceneAs()
	{
		std::string filePath = FileDialogs::saveFile("Basil Scene (*.scene)\0*.scene\0");
		if (!filePath.empty())
		{
			SceneSerializer serializer(activeScene);
			serializer.serialize(filePath);
		}
	}
}