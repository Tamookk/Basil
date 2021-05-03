/*
 * This header file declares the OpenGL-specific implementation of Framebuffer.
 */
#pragma once

#include "Renderer/Framebuffer.h"

namespace Basil
{
	class OpenGLFramebuffer : public Framebuffer
	{
		public:
			OpenGLFramebuffer(const FramebufferSpecification& spec);
			~OpenGLFramebuffer();
			void invalidate();
			void bind() override;
			void unbind() override;
			uint32_t getColorAttachmentRendererID() const override;
			const FramebufferSpecification& getSpecification() const override;
		private:
			uint32_t rendererID;
			uint32_t colorAttachment;
			uint32_t depthAttachment;
			FramebufferSpecification specification;
	};
}