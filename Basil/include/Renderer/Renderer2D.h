/*
 * This header file declares the 2D renderer class.
 */
#pragma once

#include "Renderer/OrthographicCamera.h"
#include "Renderer/Texture2D.h"
#include "Scene/Component.h"

namespace Basil
{
	class Renderer2D
	{
		public:
			struct Statistics
			{
				uint32_t drawCalls = 0;
				uint32_t quadCount = 0;
				uint32_t getTotalVertexCount();
				uint32_t getTotalIndexCount();
			};
			static void init();
			static void shutdown();
			static void beginScene(const OrthographicCamera& camera);
			static void endScene();
			static void flush();
			static void drawQuad(const TransformComponent& transform, const glm::vec4& color);
			static void drawQuad(const TransformComponent& transform, const Shared<Texture2D>& texture, float textureScale = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
			static void resetStats();
			static Statistics getStats();
		private:
			static void flushAndReset();
	};
}
