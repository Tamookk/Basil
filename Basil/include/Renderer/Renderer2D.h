/*
 * This header file declares the 2D renderer class.
 */
#pragma once

#include "Renderer/OrthographicCamera.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Transform.h"

namespace Basil
{
	class Renderer2D
	{
		public:
			static void init();
			static void shutdown();
			static void beginScene(const OrthographicCamera& camera);
			static void endScene();
			static void flush();
			static void drawQuad(const Transform& transform, const glm::vec4& color);
			static void drawQuad(const Transform& transform, const Shared<Texture2D>& texture, float textureScale = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
	};
}
