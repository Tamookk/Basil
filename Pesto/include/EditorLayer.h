/*
 * Declares a class for testing 2D rendering.
 */
#pragma once

#include "Panels/ContentBrowserPanel.h"
#include "Panels/PropertiesPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Renderer/EditorCamera.h"

#include "Core/Basil.h"

namespace Basil
{
	class EditorLayer : public Layer
	{
		public:
			enum class SceneState
			{
				Edit = 0,
				Play = 1,
				Simulate = 2
			};

		public:
			EditorLayer();
			virtual ~EditorLayer() {}
			
			void onAttach() override;
			void onDetach() override;
			void onUpdate(Timestep timeStep) override;
			void onEvent(Event& e) override;
			void onImGuiRender() override;
		private:
			bool onKeyPressed(KeyPressedEvent& e);
			bool onMouseButtonPressed(MouseButtonPressedEvent& e);

			void onOverlayRender();
			
			void newScene();
			void openScene();
			void openScene(const std::filesystem::path path);
			void saveScene();
			void saveSceneAs();
			void serializeScene(Shared<Scene> scene, const std::filesystem::path& path);

			void onScenePlay();
			void onSceneSimulate();
			void onSceneStop();

			void onDuplicateEntity();

			// -- UI -- //
			void UI_Toolbar();

			// -- Panels -- //
			ContentBrowserPanel contentBrowserPanel;
			PropertiesPanel propertiesPanel;
			SceneHierarchyPanel sceneHierarchyPanel;

			Shared<Framebuffer> framebuffer;
			Shared<Scene> activeScene;
			Shared<Scene> editorScene;
			std::filesystem::path editorScenePath;

			glm::vec2 viewportSize = { 0.0f, 0.0f };
			glm::vec2 viewportBounds[2];

			glm::vec4 squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
			
			EditorCamera editorCamera;

			Entity squareEntity;
			Entity cameraEntity;
			Entity secondCameraEntity;
			Entity hoveredEntity;
			
			bool viewportFocused;
			bool viewportHovered;
			bool primaryCamera = true;
			bool showPhysicsColliders;

			int gizmoType;

			SceneState sceneState;

			// -- Editor Resources -- //
			Shared<Texture2D> iconPlay;
			Shared<Texture2D> iconSimulate;
			Shared<Texture2D> iconStop;
	};
}