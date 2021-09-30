#include "EditorLayer.h"
#include "Core/Application.h"
#include "Math/Math.h"
#include "Scene/SceneSerializer.h"
#include "Utils/PlatfomUtils.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <ImGuizmo.h>

namespace Basil
{
	extern const std::filesystem::path assetPath;

	// Constructor
	EditorLayer::EditorLayer() : Layer("Sandbox2D")
	{
		viewportFocused = false;
		viewportHovered = false;
		gizmoType = -1;
		sceneState = SceneState::Edit;
	}

	void EditorLayer::onAttach()
	{
		PROFILE_FUNCTION();

		// Set play and stop icons
		iconPlay = Texture2D::create("res/icons/editor_play_stop/play_button.png");
		iconStop = Texture2D::create("res/icons/editor_play_stop/stop_button.png");

		// Create framebuffer
		FramebufferSpecification fbSpec;
		fbSpec.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.width = 1280;
		fbSpec.height = 720;
		framebuffer = Framebuffer::create(fbSpec);

		// Create scene
		activeScene = makeShared<Scene>();

		auto commandLineArgs = Application::get().getCommandLineArgs();
		if (commandLineArgs.count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			SceneSerializer serializer(activeScene);
			serializer.deserialize(sceneFilePath);
		}

		// Set up editor camera
		editorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

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
				editorCamera.setViewportSize(viewportSize.x, viewportSize.y);
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
			framebuffer->clearAttachment(1, -1);
		}

		// Update scene
		{
			PROFILE_SCOPE("Renderer Draw");
			switch (sceneState)
			{
				case SceneState::Edit:
				{
					PROFILE_SCOPE("Editor Update");
					editorCamera.onUpdate(timeStep);
					activeScene->onUpdateEditor(timeStep, editorCamera);
					break;
				}
				case SceneState::Play:
				{
					PROFILE_SCOPE("Play Update");
					activeScene->onUpdateRuntime(timeStep);
					break;
				}
			}

			auto [mx, my] = ImGui::GetMousePos();
			mx -= viewportBounds[0].x;
			my -= viewportBounds[0].y;
			glm::vec2 viewportSize = viewportBounds[1] - viewportBounds[0];
			my = viewportSize.y - my;
			int mouseX = (int)mx;
			int mouseY = (int)my;

			if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
			{
				int pixelData = framebuffer->readPixel(1, mouseX, mouseY);
				hoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, activeScene.get());
			}

			framebuffer->unbind();
		}
	}

	void EditorLayer::onEvent(Event& e)
	{
		// Editor camera on event
		editorCamera.onEvent(e);

		// Dispatch key pressed event
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT(EditorLayer::onKeyPressed));
		dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT(EditorLayer::onMouseButtonPressed));
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
				if (ImGui::MenuItem("Save...", "Ctrl+S"))
					saveScene();

				// Save scene file as 
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					saveSceneAs();

				// Exit application
				if (ImGui::MenuItem("Exit"))
					Application::get().close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// Content browser panel
		contentBrowserPanel.onImGuiRender();
		
		// Scene hierarchy panel
		sceneHierarchyPanel.onImGuiRender();

		// Properties panel
		propertiesPanel.onImGuiRender(sceneHierarchyPanel.getSelectedEntity());

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
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		viewportFocused = ImGui::IsWindowFocused();
		viewportHovered = ImGui::IsWindowHovered();
		
		Application::get().getImGuiLayer()->setBlockEvents(!viewportFocused && !viewportHovered);
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		uint64_t textureID = framebuffer->getColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		// Accept drag and drop data from the Content Browser panel (currently a scene file)
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* data = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)data->Data;

				// Open a scene
				openScene(std::filesystem::path(assetPath) / path);
			}

			ImGui::EndDragDropTarget();
		}
		
		// Gizmos
		Entity selectedEntity = sceneHierarchyPanel.getSelectedEntity();
		if (selectedEntity && gizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			// Set rect size
			ImGuizmo::SetRect(viewportBounds[0].x, viewportBounds[0].y, viewportBounds[1].x - viewportBounds[0].x, viewportBounds[1].y - viewportBounds[0].y);
			// Set camera (runtime camera from entity)
			//auto cameraEntity = activeScene->getPrimaryCameraEntity();
			//const auto& camera = cameraEntity.getComponent<CameraComponent>().camera;
			//const glm::mat4& cameraProjection = camera.getProjection();
			//glm::mat4 cameraView = glm::inverse(cameraEntity.getComponent<TransformComponent>().getTransform());

			// Editor camera
			const glm::mat4& cameraProjection = editorCamera.getProjection();
			glm::mat4 cameraView = editorCamera.getViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.getComponent<TransformComponent>();
			glm::mat4 transform = tc.getTransform();

			// Snapping
			bool snap = Input::isKeyPressed(Key::LeftControl);
			float snapValue = 0.5f;
			if (gizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)gizmoType,
				ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::decomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.rotation;
				tc.translation = translation;
				tc.rotation += deltaRotation;
				tc.scale = scale;
			}
		}
		
		ImGui::End();
		ImGui::PopStyleVar();

		UI_Toolbar();

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
				if (control)
					if (shift)
						saveSceneAs();
					else
						saveScene();
				break;

			// Gizmos
			case (int)Key::Q:
				if(!ImGuizmo::IsUsing())
					gizmoType = -1;
				break;

			case (int)Key::W:
				if (!ImGuizmo::IsUsing())
					gizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;

			case (int)Key::E:
				if (!ImGuizmo::IsUsing())
					gizmoType = ImGuizmo::OPERATION::ROTATE;
				break;

			case (int)Key::R:
				if (!ImGuizmo::IsUsing())
					gizmoType = ImGuizmo::OPERATION::SCALE;
				break;
		}
		return false;
	}

	// On mouse button pressed function
	bool EditorLayer::onMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.getMouseButton() == (int)Mouse::ButtonLeft)
		{
			if (viewportHovered && !ImGuizmo::IsOver() && !Input::isKeyPressed(Key::LeftAlt))
				sceneHierarchyPanel.setSelectedEntity(hoveredEntity);
		}
		return false;
	}

	// Make a new scene
	void EditorLayer::newScene()
	{
		activeScene = makeShared<Scene>();
		activeScene->onViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		sceneHierarchyPanel.setContext(activeScene);
		activeScenePath = "";
	}

	// Open a scene
	void EditorLayer::openScene()
	{
		std::string filePath = FileDialogs::openFile("Basil Scene (*.scene)\0*.scene\0");
		if (!filePath.empty())
			openScene(filePath);

	}

	// Open a scene (with a given file path)
	void EditorLayer::openScene(const std::filesystem::path path)
	{
		// Close scene if it is playing
		if (sceneState != SceneState::Edit)
			onSceneStop();

		if (path.extension().string() != ".scene")
		{
			LOG_WARN("Could not load {0} - not a scene file", path.filename().string());
			return;
		}

		Shared<Scene> newScene = makeShared<Scene>();
		SceneSerializer serializer(newScene);

		if (serializer.deserialize(path.string()))
		{
			activeScene = newScene;
			activeScene->onViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			sceneHierarchyPanel.setContext(activeScene);
			activeScenePath = path;
		}
	}

	// Save a scene
	void EditorLayer::saveScene()
	{
		if (activeScenePath.empty())
			saveSceneAs();
		else
			serializeScene(activeScenePath);
	}

	// Save a scene as
	void EditorLayer::saveSceneAs()
	{
		std::string filePath = FileDialogs::saveFile("Basil Scene (*.scene)\0*.scene\0");
		if (!filePath.empty())
		{
			serializeScene(filePath);
			activeScenePath = filePath;
		}
	}

	// Serialize a scene
	void EditorLayer::serializeScene(const std::filesystem::path& path)
	{
		ASSERT(!path.empty(), "Path is empty");

		SceneSerializer serializer(activeScene);
		serializer.serialize(path.string());
	}

	// When the scene plays
	void EditorLayer::onScenePlay()
	{
		sceneState = SceneState::Play;
		activeScene->onRuntimeStart();
	}

	// When the scene stops
	void EditorLayer::onSceneStop()
	{
		sceneState = SceneState::Edit;
		activeScene->onRuntimeStop();
	}


	// -- UI -- //
	// Create the UI toolbar
	void EditorLayer::UI_Toolbar()
	{
		// Push style
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHoveredColor = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHoveredColor.x, buttonHoveredColor.y, buttonHoveredColor.z, 0.5f));
		const auto& buttonActiveColor = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActiveColor.x, buttonActiveColor.y, buttonActiveColor.z, 0.5f));

		// Create toolbar flags
		int toolbarFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar;
		toolbarFlags |= ImGuiWindowFlags_NoScrollWithMouse;

		// Begin toolbar
		ImGui::Begin("##Toolbar", nullptr, toolbarFlags);

		// Set button size and icon
		float size = ImGui::GetWindowHeight() - 4;
		Shared<Texture2D> icon = sceneState == SceneState::Edit ? iconPlay : iconStop;
	
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5) - (size * 0.5));

		// If button pressed
		if (ImGui::ImageButton((ImTextureID)icon->getRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
		{
			if (sceneState == SceneState::Edit)
				onScenePlay();
			else if (sceneState == SceneState::Play)
				onSceneStop();
		}

		// End toolbar
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}
}