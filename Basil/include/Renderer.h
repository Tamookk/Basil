/*
 * This header decleares a class used for getting the graphics API currently in use.
 */
#pragma once

#include "OrthographicCamera.h"
#include "RendererAPI.h"
#include "Shader.h"

namespace Basil
{
	class Renderer
	{
		public:
			static void beginScene(OrthographicCamera& camera);
			static void endScene();
			static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vao);
			
			static void setClearColor(const glm::vec4& color);
			static void clear();
			static void drawIndexed(const std::shared_ptr<VertexArray>& vao);

			static RendererAPI::API getAPI();
		private:
			struct SceneData
			{
				glm::mat4 viewProjectionMatrix;
			};

			static RendererAPI* rendererAPI;
			static SceneData* sceneData;
	};
}