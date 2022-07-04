/*
 * This header files declares the OpenGL implementation of Texture2D.
 */
#pragma once

#include "Renderer/Texture2D.h"

#include <glad/glad.h>

namespace Basil
{
	class OpenGLTexture2D : public Texture2D
	{
		public:
			OpenGLTexture2D(const std::string& path);
			OpenGLTexture2D(uint32_t width, uint32_t height);
			~OpenGLTexture2D();
			uint32_t getWidth() const override;
			uint32_t getHeight() const override;
			uint32_t getRendererID() const override;
			const std::string& getPath() const override;
			void setData(void* data, uint32_t size) override;
			void bind(unsigned int slot = 0) const override;
			bool isTextureLoaded() const override;
			bool operator==(const Texture& other) const override;
		private:
			std::string path;
			bool isLoaded;
			uint32_t width;
			uint32_t height;
			uint32_t rendererID;
			GLenum internalFormat;
			GLenum dataFormat;
	};
}