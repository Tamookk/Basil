#include "pch.h"
#include "BufferElement.h"

// Function to get the size of a shader data type
static uint32_t ShaderDataTypeSize(ShaderDataType type)
{
	switch (type)
	{
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 4 * 2;
		case ShaderDataType::Float3:	return 4 * 3;
		case ShaderDataType::Float4:	return 4 * 4;
		case ShaderDataType::Mat3:		return 4 * 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4 * 4;
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::Int2:		return 4 * 2;
		case ShaderDataType::Int3:		return 4 * 3;
		case ShaderDataType::Int4:		return 4 * 4;
		case ShaderDataType::Bool:		return 1;
	}

	ASSERT(false, "Unknown ShaderDataType!");
	return 0;
}

// BufferElement constructor
BufferElement::BufferElement(const std::string& name, ShaderDataType type, bool normalised)
{
	this->name = name;
	this->type = type;
	size = ShaderDataTypeSize(type);
	offset = 0;
	this->normalised = normalised;
}

// Get component count of given shader data type
uint32_t BufferElement::getComponentCount() const
{
	switch (type)
	{
		case ShaderDataType::Float:		return 1;
		case ShaderDataType::Float2:	return 2;
		case ShaderDataType::Float3:	return 3;
		case ShaderDataType::Float4:	return 4;
		case ShaderDataType::Mat3:		return 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4;
		case ShaderDataType::Int:		return 1;
		case ShaderDataType::Int2:		return 2;
		case ShaderDataType::Int3:		return 3;
		case ShaderDataType::Int4:		return 4;
		case ShaderDataType::Bool:		return 1;
	}

	ASSERT(false, "Unknown ShaderDataType!");
	return 0;
}