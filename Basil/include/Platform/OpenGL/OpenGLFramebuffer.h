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
			void resize(uint32_t width, uint32_t height) override;
			int readPixel(uint32_t attachmentIndex, int x, int y) override;
			uint32_t getColorAttachmentRendererID(uint32_t index = 0) const override;
			const FramebufferSpecification& getSpecification() const override;
		private:
			uint32_t rendererID;
			FramebufferSpecification specification;

			std::vector<FramebufferTextureSpecification> colorAttachmentSpecifications;
			FramebufferTextureSpecification depthAttachmentSpecification;

			std::vector<uint32_t> colorAttachments;
			uint32_t depthAttachment;
	};
}