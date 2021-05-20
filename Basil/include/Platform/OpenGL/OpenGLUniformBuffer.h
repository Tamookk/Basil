/*
 * This header declares the OpenGL implementation of UniformBuffer.
 */
#pragma once

#include "Renderer/UniformBuffer.h"

namespace Basil
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
		public:
			OpenGLUniformBuffer(uint32_t size, uint32_t binding);
			~OpenGLUniformBuffer();
			void setData(const void* data, uint32_t size, uint32_t offset = 0) override;
		private:
			uint32_t rendererID;
	};
}