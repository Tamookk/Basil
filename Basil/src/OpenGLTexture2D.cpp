#include "pch.h"
#include "OpenGLTexture2D.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Basil
{
	// Constructor
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	{
		// Initialise path variable
		this->path = path;

		// Load image and initialise width & height variables
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		ASSERT(data, "Failed to load image.");
		this->width = width;
		this->height = height;

		// Create texture in OpenGL
		glCreateTextures(GL_TEXTURE_2D, 1, &rendererID);
		glTextureStorage2D(rendererID, 1, GL_RGB8, this->width, this->height);

		// Set texture parameters
		glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Upload texture
		glTextureSubImage2D(rendererID, 0, 0, 0, this->width, this->height, GL_RGB, GL_UNSIGNED_BYTE, data);

		// Free the data from memory
		stbi_image_free(data);
	}

	// Destructor
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &rendererID);
	}

	// Return the width
	uint32_t OpenGLTexture2D::getWidth() const
	{
		return width;
	}

	// Return the height
	uint32_t OpenGLTexture2D::getHeight() const
	{
		return height;
	}

	// Bind the texture to the provided slot
	void OpenGLTexture2D::bind(unsigned int slot) const
	{
		glBindTextureUnit(slot, rendererID);
	}
}