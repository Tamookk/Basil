/*
 * Declares a class for testing 2D rendering.
 */
#pragma once

#include "Core/Basil.h"

class EditorLayer : public Basil::Layer
{
	public:
		EditorLayer();
		virtual ~EditorLayer() {}
		void onAttach() override;
		void onDetach() override;
		void onUpdate(Basil::Timestep timeStep) override;
		void onEvent(Basil::Event& e) override;
		void onImGuiRender() override;
	private:
		Basil::OrthographicCameraController cameraController;
		Basil::Shared<Basil::VertexArray> vao;
		Basil::Shared<Basil::Shader> shader;
		Basil::Shared<Basil::Texture2D> texture;
		Basil::Shared<Basil::Framebuffer> framebuffer;
		glm::vec4 squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		glm::vec2 viewportSize = { 0.0f, 0.0f };
		Basil::Shared<Basil::Scene> activeScene;
		entt::entity squareEntity;
		bool viewportFocused;
		bool viewportHovered;
};
