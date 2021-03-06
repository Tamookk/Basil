/*
 * This header file contains the base class for 2D textures.
 */
#pragma once

#include <string>

#include "Core/Core.h"
#include "Renderer/Texture.h"

namespace Basil
{
	class Texture2D : public Texture
	{
		public:
			static Shared<Texture2D> create(const std::string& path);
			static Shared<Texture2D> create(uint32_t width, uint32_t height);
	};
}