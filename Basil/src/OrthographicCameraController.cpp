#include "pch.h"
#include "Input.h"
#include "KeyCodes.h"
#include "OrthographicCameraController.h"

namespace Basil
{
	// Constructor
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: camera(-aspectRatio * zoomLevel, aspectRatio* zoomLevel, -zoomLevel, zoomLevel)
	{
		// Initialise variables
		this->aspectRatio = aspectRatio;
		this->rotation = rotation;
		cameraRotation = 0.0f;
		cameraXSpeed = 2.0f;
		cameraYSpeed = 2.0f;
		cameraRotateSpeed = 90.0f;
	}

	// On update function
	void OrthographicCameraController::onUpdate(Timestep timeStep)
	{
		// Poll for input
		if (Input::isKeyPressed(BASIL_KEY_D))
			cameraPosition.x += cameraXSpeed * timeStep;
		else if (Input::isKeyPressed(BASIL_KEY_A))
			cameraPosition.x -= cameraXSpeed * timeStep;

		if (Input::isKeyPressed(BASIL_KEY_W))
			cameraPosition.y += cameraYSpeed * timeStep;
		else if (Input::isKeyPressed(BASIL_KEY_S))
			cameraPosition.y -= cameraYSpeed * timeStep;

		// Set the camera's rotation if camera rotation is enabled
		if (rotation)
		{
			if (Input::isKeyPressed(BASIL_KEY_LEFT))
				cameraRotation += cameraRotateSpeed * timeStep;
			else if (Input::isKeyPressed(BASIL_KEY_RIGHT))
				cameraRotation -= cameraRotateSpeed * timeStep;

			camera.setRotation(cameraRotation);
		}

		// Set the camera's position
		camera.setPosition(cameraPosition);

		// Set translation speed
		cameraXSpeed = 2.0f * zoomLevel;
		cameraYSpeed = 2.0f * zoomLevel;
	}

	// On event function
	void OrthographicCameraController::onEvent(Event& e)
	{
		Basil::EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT(OrthographicCameraController::onMouseScrolled));
		dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT(OrthographicCameraController::onWindowResize));
	}

	// Return the camera
	OrthographicCamera& OrthographicCameraController::getCamera()
	{
		return camera;
	}

	// Return the camera, but const
	const OrthographicCamera& OrthographicCameraController::getCamera() const
	{
		return camera;
	}

	// Get the zoom level
	float OrthographicCameraController::getZoomLevel() const
	{
		return zoomLevel;
	}

	// Set the zoom level
	void OrthographicCameraController::setZoomLevel(float zoomLevel)
	{
		this->zoomLevel = zoomLevel;
	}

	// Mouse scrolled event
	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e)
	{
		zoomLevel -= e.getYOffset() * 0.25f;
		zoomLevel = std::max(zoomLevel, 0.25f);
		camera.setProjectionMatrix(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
		return false;
	}

	// Window resize event
	bool OrthographicCameraController::onWindowResize(WindowResizeEvent& e)
	{
		aspectRatio = (float)e.getWidth() / (float)e.getHeight();
		camera.setProjectionMatrix(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
		return false;
	}
}