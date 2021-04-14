/*
 * This header files declares the OpenGL implementation of Texture2D
 */
#pragma once

#include "Texture2D.h"

namespace Basil
{
	class OpenGLTexture2D : public Texture2D
	{
		public:
			OpenGLTexture2D(const std::string& path);
			~OpenGLTexture2D();
			uint32_t getWidth() const override;
			uint32_t getHeight() const override;
			void bind(unsigned int slot = 0) const override;
		private:
			std::string path;
			uint32_t width;
			uint32_t height;
			uint32_t rendererID;
			unsigned int internalFormat;
			unsigned int dataFormat;
	};
}