/*
 * This header decleares the base renderer class.
 */
#pragma once

#include "Renderer/OrthographicCamera.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/Shader.h"

namespace Basil
{
	class Renderer
	{
		public:
			static void init();
			static void shutdown();
			static void onWindowResize(unsigned int width, unsigned int height);
			static void beginScene(OrthographicCamera& camera);
			static void endScene();
			static void submit(const Shared<Shader>& shader, const Shared<VertexArray>& vao, const glm::mat4& transform = glm::mat4(1.0f));
			static void setClearColor(const glm::vec4& color);
			static void clear();
			static void drawIndexed(const Shared<VertexArray>& vao, uint32_t indexCount = 0);
			static RendererAPI::API getAPI();
		private:
			struct SceneData
			{
				glm::mat4 viewProjectionMatrix;
			};
			static Unique<RendererAPI> rendererAPI;
			static Unique<SceneData> sceneData;
	};
}