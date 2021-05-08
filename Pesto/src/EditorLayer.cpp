#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Basil
{
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
		texture = Texture2D::create("assets/textures/test.png");

		// Create framebuffer
		FramebufferSpecification fbSpec;
		fbSpec.width = 1280;
		fbSpec.height = 720;
		framebuffer = Framebuffer::create(fbSpec);

		// Create scene
		activeScene = makeShared<Scene>();

		// Create square entity
		auto square = activeScene->createEntity("Green Square");
		square.addComponent<SpriteRenderComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
		squareEntity = square;

		cameraEntity = activeScene->createEntity("Camera");
		cameraEntity.addComponent<CameraComponent>();

		secondCameraEntity = activeScene->createEntity("Clip-Space Entity");
		auto& cc = secondCameraEntity.addComponent<CameraComponent>();
		cc.primary = false;

		class CameraController : public ScriptableEntity
		{
			public:
				void onCreate() {}

				void onDestroy() {}

				void onUpdate(Timestep timeStep)
				{
					auto& transform = getComponent<TransformComponent>();
					float speed = 5.0f;

					if (Input::isKeyPressed(KeyCode::A))
						transform.position.x -= speed * timeStep;
					if (Input::isKeyPressed(KeyCode::D))
						transform.position.x += speed * timeStep;
					if (Input::isKeyPressed(KeyCode::W))
						transform.position.y += speed * timeStep;
					if (Input::isKeyPressed(KeyCode::S))
						transform.position.y -= speed * timeStep;
				}
		};

		cameraEntity.addComponent<NativeScriptComponent>().bind<CameraController>();

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
				cameraController.resize(viewportSize.x, viewportSize.y);

				activeScene->onViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			}
		}

		// Update camera if viewport is focused
		if (viewportFocused)
			cameraController.onUpdate(timeStep);

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
			Renderer2D::beginScene(cameraController.getCamera());

			TransformComponent transform;
			Renderer2D::drawQuad(transform, squareColor);

			transform.position = { -1.0f, 0.5f, 0.0f };
			transform.scale = { 0.5f, 0.5f, 1.0f };
			Renderer2D::drawQuad(transform, { 1.0f, 0.0f, 0.0f, 1.0f });

			transform.position = { 0.0f, 0.0f, -0.1f };
			transform.rotation = 45.0f;
			transform.scale = { 10.0f, 10.0f, 1.0f };
			Renderer2D::drawQuad(transform, texture, 10.0f);

			Renderer2D::endScene();

			Renderer2D::beginScene(cameraController.getCamera());
			transform.rotation = 0.0f;
			transform.scale = { 1.0f, 1.0f, 1.0f };
			for (float y = -5.0f; y < 5.0f; y += 0.5f)
			{
				for (float x = -5.0f; x < 5.0f; x += 0.5f)
				{
					glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
					transform.position = { x, y, 0.0f };
					Renderer2D::drawQuad(transform, color);
				}
			}
			Renderer2D::endScene();
			framebuffer->unbind();
		}
	}

	void EditorLayer::onEvent(Event& e)
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
					Application::get().close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}


		sceneHierarchyPanel.onImGuiRender();

		// Settings panel
		ImGui::Begin("Settings");
		Renderer2D::Statistics stats = Renderer2D::getStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quadCount);
		ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
		ImGui::Text("Indices: %d", stats.getTotalIndexCount());
		ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));

		if (squareEntity)
		{
			ImGui::Separator();
			auto& tag = squareEntity.getComponent<TagComponent>().tag;
			ImGui::Text("%s", tag.c_str());
			auto& greenSquareColor = squareEntity.getComponent<SpriteRenderComponent>().color;
			ImGui::ColorEdit4("Green Square Color", glm::value_ptr(greenSquareColor));
			ImGui::Separator();
		}

		ImGui::DragFloat3("Camera Transform", glm::value_ptr(cameraEntity.getComponent<TransformComponent>().position));
		if (ImGui::Checkbox("Camera A", &primaryCamera))
		{
			cameraEntity.getComponent<CameraComponent>().primary = primaryCamera;
			secondCameraEntity.getComponent<CameraComponent>().primary = !primaryCamera;
		}

		{
			auto& camera = secondCameraEntity.getComponent<CameraComponent>().camera;
			float orthoSize = camera.getOrthographicSize();
			if (ImGui::DragFloat("Second Camera Ortho Size", &orthoSize))
				camera.setOrthographicSize(orthoSize);
		}

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
}