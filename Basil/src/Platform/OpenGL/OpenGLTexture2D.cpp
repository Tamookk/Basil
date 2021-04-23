#include "pch.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Basil
{
	// Constructor
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	{
		PROFILE_FUNCTION();

		// Initialise variables
		this->path = path;
		internalFormat = 0;
		dataFormat = 0;

		// Load image and initialise width & height variables
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data;
		{
			PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		ASSERT(data, "Failed to load image.");
		this->width = width;
		this->height = height;

		// Set internal and data format based on number of channels in image
		switch (channels)
		{
			case 3:
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
				break;
			case 4:
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
				break;
			default:
				ASSERT(internalFormat & dataFormat, "Format not supported!");
				break;
		}

		// Create texture in OpenGL
		glCreateTextures(GL_TEXTURE_2D, 1, &rendererID);
		glTextureStorage2D(rendererID, 1, internalFormat, this->width, this->height);

		// Set texture parameters
		glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Upload texture
		glTextureSubImage2D(rendererID, 0, 0, 0, this->width, this->height, dataFormat, GL_UNSIGNED_BYTE, data);

		// Free the data from memory
		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
	{
		PROFILE_FUNCTION();

		// Set variables
		this->width = width;
		this->height = height;
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;

		// Create texture in OpenGL
		glCreateTextures(GL_TEXTURE_2D, 1, &rendererID);
		glTextureStorage2D(rendererID, 1, internalFormat, this->width, this->height);

		// Set texture parameters
		glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	// Destructor
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		PROFILE_FUNCTION();

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

	// Set texture data
	void OpenGLTexture2D::setData(void* data, uint32_t size)
	{
		PROFILE_FUNCTION();

		uint32_t bytesPerPixel = dataFormat == GL_RGBA ? 4 : 3;
		ASSERT(size == width * height * bytesPerPixel, "Data must be entire texture!");
		glTextureSubImage2D(rendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
	}

	// Bind the texture to the provided slot
	void OpenGLTexture2D::bind(unsigned int slot) const
	{
		PROFILE_FUNCTION();

		glBindTextureUnit(slot, rendererID);
	}
}