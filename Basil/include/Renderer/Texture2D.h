/*
 * This header file contains the base class for 2D textures.
 */
#pragma once

#include <string>

#include "Core.h"
#include "Texture.h"

namespace Basil
{
	class Texture2D : public Texture
	{
		public:
			static Shared<Texture2D> create(const std::string& path);
	};
}