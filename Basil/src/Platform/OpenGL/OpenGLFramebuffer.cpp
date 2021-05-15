#include "pch.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Basil
{
	// OpenGL framebuffer utils
	namespace Utils
	{
		// Return texture target
		static GLenum textureTarget(bool multiSampled)
		{
			return multiSampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		// Create textures
		static void createTextures(bool multiSampled, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(textureTarget(multiSampled), count, outID);
		}

		// Bind texture
		static void bindTexture(bool multiSampled, uint32_t id)
		{
			glBindTexture(textureTarget(multiSampled), id);
		}

		// Attach a color texture
		static void attachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multiSampled = samples > 1;
			if (multiSampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, textureTarget(multiSampled), id, 0);
		}

		// Attach a depth texture
		static void attachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multiSampled = samples > 1;
			if (multiSampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, textureTarget(multiSampled), id, 0);
		}

		// Check if depth format
		static bool isDepthFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
				case FramebufferTextureFormat::DEPTH24STENCIL8:
					return true;
			}

			return false;
		}
	}

	// Constructor
	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
	{
		// Initialise variables
		specification = spec;
		rendererID = 0;
		depthAttachmentSpecification = FramebufferTextureFormat::None;
		depthAttachment = 0;

		for (auto spec : specification.attachments.attachments)
		{
			if (!Utils::isDepthFormat(spec.textureFormat))
				colorAttachmentSpecifications.emplace_back(spec);
			else
				depthAttachmentSpecification = spec;
		}
		
		// Create frame buffer
		invalidate();
	}

	// Destructor
	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &rendererID);
		glDeleteTextures(colorAttachments.size(), colorAttachments.data());
		glDeleteTextures(1, &depthAttachment);
	}
	
	// Invalidate the framebuffer
	void OpenGLFramebuffer::invalidate()
	{
		if (rendererID)
		{
			glDeleteFramebuffers(1, &rendererID);
			glDeleteTextures(colorAttachments.size(), colorAttachments.data());
			glDeleteTextures(1, &depthAttachment);

			colorAttachments.clear();
			depthAttachment = 0;
		}

		// Create and bind OpenGL framebuffer
		glCreateFramebuffers(1, &rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, rendererID);

		bool multiSample = specification.samples > 1;

		// Attachments
		if (colorAttachmentSpecifications.size())
		{
			colorAttachments.resize(colorAttachmentSpecifications.size());
			Utils::createTextures(multiSample, colorAttachments.data(), colorAttachments.size());

			for (size_t i = 0; i < colorAttachments.size(); i++)
			{
				Utils::bindTexture(multiSample, colorAttachments[i]);
				switch (colorAttachmentSpecifications[i].textureFormat)
				{
					case FramebufferTextureFormat::RGBA8:
						Utils::attachColorTexture(colorAttachments[i], specification.samples, GL_RGBA8, GL_RGBA, specification.width, specification.height, i);
						break;
					case FramebufferTextureFormat::RED_INTEGER:
						Utils::attachColorTexture(colorAttachments[i], specification.samples, GL_R32I, GL_RED_INTEGER, specification.width, specification.height, i);
				}
			}
		}

		if (depthAttachmentSpecification.textureFormat != FramebufferTextureFormat::None)
		{
			Utils::createTextures(multiSample, &depthAttachment, 1);
			Utils::bindTexture(multiSample, depthAttachment);
			switch (depthAttachmentSpecification.textureFormat)
			{
				case FramebufferTextureFormat::DEPTH24STENCIL8:
					Utils::attachDepthTexture(depthAttachment, specification.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, specification.width, specification.height);
					break;
			}
		}

		if (colorAttachments.size() > 1)
		{
			ASSERT(colorAttachments.size() <= 4, "");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };

			glDrawBuffers(colorAttachments.size(), buffers);
		}
		else if (colorAttachments.empty())
		{
			glDrawBuffer(GL_NONE);
		}

		ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete.");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// Bind the framebuffer
	void OpenGLFramebuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, rendererID);
		glViewport(0, 0, specification.width, specification.height);
	}

	// Unbind the framebuffer
	void OpenGLFramebuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// Resize the frame buffer
	void OpenGLFramebuffer::resize(uint32_t width, uint32_t height)
	{
		if (width <= 0 || height <= 0)
		{
			LOG_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
			return;
		}
		specification.width = width;
		specification.height = height;
		invalidate();
	}

	// Read pixel entity value
	int OpenGLFramebuffer::readPixel(uint32_t attachmentIndex, int x, int y)
	{
		ASSERT(attachmentIndex < colorAttachments.size(), "");

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	// Return the color attachment renderer ID
	uint32_t OpenGLFramebuffer::getColorAttachmentRendererID(uint32_t index) const
	{
		ASSERT(index < colorAttachments.size(), "");
		return colorAttachments[index];
	}

	// Return the Framebuffer Specification
	const FramebufferSpecification& OpenGLFramebuffer::getSpecification() const
	{
		return specification;
	}
}