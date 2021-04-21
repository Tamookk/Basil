/*
 * This header file contains the base class for all textures.
 */
#pragma once

namespace Basil
{
	class Texture
	{
		public:
			virtual ~Texture() {}
			virtual uint32_t getWidth() const = 0;
			virtual uint32_t getHeight() const = 0;
			virtual void bind(unsigned int slot = 0) const = 0;
	};
}