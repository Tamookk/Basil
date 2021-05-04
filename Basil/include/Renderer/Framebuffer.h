/*
 * This header file declares a class for a basic frame buffer. 
 */
#pragma once

#include "Core/Core.h"

namespace Basil
{
	struct FramebufferSpecification
	{
		uint32_t width;
		uint32_t height;
		uint32_t samples = 1;
		bool swapChainTarget = false;
	};

	class Framebuffer
	{
		public:
			virtual void bind() = 0;
			virtual void unbind() = 0;
			virtual void resize(uint32_t width, uint32_t height) = 0;
			virtual uint32_t getColorAttachmentRendererID() const = 0;
			virtual const FramebufferSpecification& getSpecification() const = 0;
			static Shared<Framebuffer> create(const FramebufferSpecification& spec);
	};
}