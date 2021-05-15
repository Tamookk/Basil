/*
 * Declares a class for testing 2D rendering.
 */
#pragma once

#include "Panels/PropertiesPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Renderer/EditorCamera.h"

#include "Core/Basil.h"

namespace Basil
{
	class EditorLayer : public Layer
	{
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
			
			void newScene();
			void openScene();
			void saveSceneAs();

			PropertiesPanel propertiesPanel;
			SceneHierarchyPanel sceneHierarchyPanel;
			Shared<Framebuffer> framebuffer;
			Shared<Scene> activeScene;

			glm::vec2 viewportSize = { 0.0f, 0.0f };
			glm::vec2 viewportBounds[2];

			glm::vec4 squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
			
			EditorCamera editorCamera;

			Entity squareEntity;
			Entity cameraEntity;
			Entity secondCameraEntity;
			
			bool viewportFocused;
			bool viewportHovered;
			bool primaryCamera = true;

			int gizmoType;
	};
}