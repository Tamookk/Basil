/*
 * This header declares a class that is a wrapper around OrthographicCamera.
 */
#pragma once

#include "Core/Timestep.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Renderer/OrthographicCamera.h"

namespace Basil
{
	class OrthographicCameraController
	{
		public:
			OrthographicCameraController(float aspectRatio, bool rotation = false);
			void onUpdate(Timestep timeStep);
			void onEvent(Event& e);
			OrthographicCamera& getCamera();
			const OrthographicCamera& getCamera() const;
			float getZoomLevel() const;
			void setZoomLevel(float zoomLevel);
		private:
			bool onMouseScrolled(MouseScrolledEvent& e);
			bool onWindowResize(WindowResizeEvent& e);
			float aspectRatio;
			float zoomLevel = 1.0f;
			OrthographicCamera camera;
			bool rotation;
			glm::vec3 cameraPosition = { 0.0f, 0.0f, 0.0f };
			float cameraRotation;
			float cameraXSpeed;
			float cameraYSpeed;
			float cameraRotateSpeed;
	};
}