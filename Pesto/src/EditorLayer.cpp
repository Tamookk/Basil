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
		showPhysicsColliders = false;
		gizmoType = -1;
		sceneState = SceneState::Edit;
	}

	void EditorLayer::onAttach()
	{
		PROFILE_FUNCTION();

		// Set play, simulate, and stop icons
		iconPlay = Texture2D::create("res/icons/editor_play_stop/play_button.png");
		iconSimulate = Texture2D::create("res/icons/editor_play_stop/simulate_button.png");
		iconStop = Texture2D::create("res/icons/editor_play_stop/stop_button.png");

		// Create framebuffer
		FramebufferSpecification fbSpec;
		fbSpec.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.width = 1280;
		fbSpec.height = 720;
		framebuffer = Framebuffer::create(fbSpec);

		// Create scene
		editorScene = makeShared<Scene>();
		activeScene = editorScene;

		auto commandLineArgs = Application::get().getSpecification().applicationCommandLineArgs;
		if (commandLineArgs.count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			SceneSerializer serializer(activeScene);
			serializer.deserialize(sceneFilePath);
		}

		// Set up editor camera
		editorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		// Set line width
		Renderer2D::setLineWidth(4.0f);

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
				case SceneState::Simulate:
				{
					PROFILE_SCOPE("Simulate Update");
					editorCamera.onUpdate(timeStep);
					activeScene->onUpdateSimulation(timeStep, editorCamera);
					break;
				}
			}

			// For mouse picking entities
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

			// Render the overlay
			onOverlayRender();

			framebuffer->unbind();
		}
	}

	void EditorLayer::onEvent(Event& e)
	{
		// Editor camera on event
		if (sceneState == SceneState::Edit)
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

		// Stats panel
		ImGui::Begin("Stats");
		Renderer2D::Statistics stats = Renderer2D::getStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quadCount);
		ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
		ImGui::Text("Indices: %d", stats.getTotalIndexCount());
		ImGui::End();


		// Settings panel
		ImGui::Begin("Settings");
		ImGui::Checkbox("Show Physics Colliders", &showPhysicsColliders);
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
		if (e.isRepeating())
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
			case (int)Key::D:
				if (control)
					onDuplicateEntity();
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

	// On overlay render function
	void EditorLayer::onOverlayRender()
	{
		// Start a new batch to render physics colliders
		if (sceneState == SceneState::Play)
		{
			Entity camera = activeScene->getPrimaryCameraEntity();

			// Return if there is no camera
			if (!camera)
				return;

			Renderer2D::beginScene(camera.getComponent<CameraComponent>().camera, camera.getComponent<TransformComponent>().getTransform());
		}
		else
		{
			Renderer2D::beginScene(editorCamera);
		}

		// If visualising physics colliders is enabled
		if (showPhysicsColliders)
		{
			// Render box colliders
			{
				PROFILE_SCOPE("Render Box Colliders");
				
				// Get all components with box collider 2D components
				auto view = activeScene->getAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
				for (auto entity : view)
				{
					// Get the transform and box collider 2D components for the entity
					auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

					// Calculate the translation, scale, and transform of the box collider
					glm::vec3 translation = tc.translation + glm::vec3(bc2d.offset, 0.001f);
					glm::vec3 scale = tc.scale * glm::vec3(bc2d.size * 2.0f, 1.0f);
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
						* glm::rotate(glm::mat4(1.0f), tc.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
						* glm::scale(glm::mat4(1.0f), scale);

					// Render the box collider visualisation
					Renderer2D::drawRect(transform, glm::vec4(0, 1, 0, 1));
				}
			}

			// Render circle colliders
			{
				PROFILE_SCOPE("Render Circle Colliders");
				
				// Get all components with circle collider 2D components
				auto view = activeScene->getAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
				for (auto entity : view)
				{
					// Get the transform and circle collider 2D components for the entity
					auto [tc, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);

					// Calculate the translation, scale, and transform of the box collider
					glm::vec3 translation = tc.translation + glm::vec3(cc2d.offset, 0.001f);
					glm::vec3 scale = tc.scale * glm::vec3(cc2d.radius * 2.0f);
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
						* glm::scale(glm::mat4(1.0f), scale);

					// Render the circle collider visualisation
					Renderer2D::drawCircle(transform, glm::vec4(0, 1, 0, 1), 0.01f);
				}
			}
		}
		
		// Draw outline over the selected entity
		if (Entity selectedEntity = sceneHierarchyPanel.getSelectedEntity())
		{
			// Get transform of the selected entity and draw an orange outline around it
			const TransformComponent transform = selectedEntity.getComponent<TransformComponent>();
			Renderer2D::drawRect(transform.getTransform(), glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
		}


		// Finish the batch
		Renderer2D::endScene();
	}

	// Make a new scene
	void EditorLayer::newScene()
	{
		activeScene = makeShared<Scene>();
		activeScene->onViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		sceneHierarchyPanel.setContext(activeScene);
		editorScenePath = std::filesystem::path();
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
			editorScene = newScene;
			editorScene->onViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			sceneHierarchyPanel.setContext(editorScene);

			activeScene = editorScene;
			editorScenePath = path;
		}
	}

	// Save a scene
	void EditorLayer::saveScene()
	{
		if (!editorScenePath.empty())
			serializeScene(activeScene, editorScenePath);
		else
			saveSceneAs();
	}

	// Save a scene as
	void EditorLayer::saveSceneAs()
	{
		std::string filePath = FileDialogs::saveFile("Basil Scene (*.scene)\0*.scene\0");
		if (!filePath.empty())
		{
			serializeScene(activeScene, filePath);
			editorScenePath = filePath;
		}
	}

	// Serialize a scene
	void EditorLayer::serializeScene(Shared<Scene> scene, const std::filesystem::path& path)
	{
		SceneSerializer serializer(scene);
		serializer.serialize(path.string());
	}

	// When the scene plays
	void EditorLayer::onScenePlay()
	{
		// Stop playing if the state is simulate
		if (sceneState == SceneState::Simulate)
			onSceneStop();

		sceneState = SceneState::Play;

		activeScene = Scene::copy(editorScene);
		activeScene->onRuntimeStart();

		sceneHierarchyPanel.setContext(activeScene);
	}

	// When physics simulation starts
	void EditorLayer::onSceneSimulate()
	{
		// Stop the scene if we have changed from playing
		if (sceneState == SceneState::Play)
			onSceneStop();

		// Set the current scene state to simulate
		sceneState = SceneState::Simulate;

		// Copy the editor scene and start the simulation
		activeScene = Scene::copy(editorScene);
		activeScene->onSimulationStart();
		
		// Set the context
		sceneHierarchyPanel.setContext(activeScene);
	}

	// When the scene stops
	void EditorLayer::onSceneStop()
	{
		// Assert we are stopping a playing or simulating scene
		ASSERT(sceneState == SceneState::Play || sceneState == SceneState::Simulate, "");

		if (sceneState == SceneState::Play)
			activeScene->onRuntimeStop();
		else if (sceneState == SceneState::Simulate)
			activeScene->onSimulationStop();

		sceneState = SceneState::Edit;

		activeScene->onRuntimeStop();
		activeScene = editorScene;

		sceneHierarchyPanel.setContext(activeScene);
	}

	// When an entity is duplicated
	void EditorLayer::onDuplicateEntity()
	{
		if (sceneState != SceneState::Edit)
			return;

		Entity selectedEntity = sceneHierarchyPanel.getSelectedEntity();
		if (selectedEntity)
			editorScene->duplicateEntity(selectedEntity);
		
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
		
		// Check if there is an active scene
		bool toolbarEnabled = (bool)activeScene;
		ImVec4 tintColor = ImVec4(1, 1, 1, 1);

		// If there is not an active scene, 
		if (!toolbarEnabled)
			tintColor.w = 0.5f;

		// Set button size
		float size = ImGui::GetWindowHeight() - 4;
		
		// Draw play button
		{
			// Set icon to play button if we are not playing, stop button otherwise
			Shared<Texture2D> icon = (sceneState == SceneState::Edit || sceneState == SceneState::Simulate) ? iconPlay : iconStop;

			// Draw the play button in the top middle of the window
			ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

			// If the play button is pressed and the toolbar is enabled
			if (ImGui::ImageButton((ImTextureID)icon->getRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), tintColor) && toolbarEnabled)
			{
				if (sceneState == SceneState::Edit || sceneState == SceneState::Simulate)
					onScenePlay();
				else if (sceneState == SceneState::Play)
					onSceneStop();
			}
		}

		ImGui::SameLine();

		// Draw physics simulation button
		{
			// Set icon to simulate button if we are not simulating, stop button otherwise
			Shared<Texture2D> icon = (sceneState == SceneState::Edit || sceneState == SceneState::Play) ? iconSimulate : iconStop;

			// If the simulate button is pressed and the toolbar is enabled
			if (ImGui::ImageButton((ImTextureID)icon->getRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), tintColor) && toolbarEnabled)
			{
				if (sceneState == SceneState::Edit || sceneState == SceneState::Play)
					onSceneSimulate();
				else if (sceneState == SceneState::Simulate)
					onSceneStop();
			}
		}

		// End toolbar
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}
}