#include "pch.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Basil
{
	// Constructor
	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
	{
		// Initialise variables
		specification = spec;

		// Invalidate previous framebuffer
		invalidate();
	}

	// Destructor
	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &rendererID);
	}
	
	// Invalidate the framebuffer
	void OpenGLFramebuffer::invalidate()
	{
		// Create and bind OpenGL framebuffer
		glCreateFramebuffers(1, &rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, rendererID);

		// Create color attachment texture
		glCreateTextures(GL_TEXTURE_2D, 1, &colorAttachment);
		glBindTexture(GL_TEXTURE_2D, colorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, specification.width, specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);

		// Create depth attachment texture
		glCreateTextures(GL_TEXTURE_2D, 1, &depthAttachment);
		glBindTexture(GL_TEXTURE_2D, depthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, specification.width, specification.height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);

		ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete.");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// Bind the framebuffer
	void OpenGLFramebuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, rendererID);
	}

	// Unbind the framebuffer
	void OpenGLFramebuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// Return the color attachment renderer ID
	uint32_t OpenGLFramebuffer::getColorAttachmentRendererID() const
	{
		return colorAttachment;
	}

	// Return the Framebuffer Specification
	const FramebufferSpecification& OpenGLFramebuffer::getSpecification() const
	{
		return specification;
	}
}