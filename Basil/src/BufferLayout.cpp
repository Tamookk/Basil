#include "pch.h"
#include "BufferLayout.h"

namespace Basil
{
	// Constructors
	BufferLayout::BufferLayout() {}

	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
	{
		this->elements = elements;
		stride = 0;
		calculateOffsetAndStride();
	}

	// Get the stride
	uint32_t BufferLayout::getStride() const
	{
		return stride;
	}

	// Get the elements
	const std::vector<BufferElement>& BufferLayout::getElements() const
	{
		return elements;
	}

	// Return the beginning of the elements vector
	std::vector<BufferElement>::iterator BufferLayout::begin()
	{
		return elements.begin();
	}

	std::vector<BufferElement>::const_iterator BufferLayout::begin() const
	{
		return elements.begin();
	}

	// Return the end of the elements vector
	std::vector<BufferElement>::iterator BufferLayout::end()
	{
		return elements.end();
	}

	std::vector<BufferElement>::const_iterator BufferLayout::end() const
	{
		return elements.end();
	}
	
	// Calculate the offset and stride of the layout
	void BufferLayout::calculateOffsetAndStride()
	{
		uint32_t offset = 0;
		stride = 0;
		for (BufferElement& element : elements)
		{
			element.offset = offset;
			offset += element.size;
			stride += element.size;
		}
	}
}