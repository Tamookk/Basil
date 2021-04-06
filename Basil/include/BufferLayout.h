/*
 * This header file declares a class used for specifying the layout of buffers.
 */
#pragma once

#include <vector>

#include "BufferElement.h"
#include "Core.h"

namespace Basil
{
	class BufferLayout
	{
		public:
			BufferLayout();
			BufferLayout(const std::initializer_list<BufferElement>& elements);
			uint32_t getStride() const;
			const std::vector<BufferElement>& getElements() const;
			std::vector<BufferElement>::iterator begin();
			std::vector<BufferElement>::const_iterator begin() const;
			std::vector<BufferElement>::iterator end();
			std::vector<BufferElement>::const_iterator end() const;
		private:
			void calculateOffsetAndStride();
			std::vector<BufferElement> elements;
			uint32_t stride;
	};
}