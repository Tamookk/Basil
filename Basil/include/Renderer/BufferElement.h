/*
 * This header file declares a class used for holding data on a buffer element.
 * This header file also defines an enum class that defines the different
 * types of data that can be passed to shaders.
 */
#pragma once

#include <cstdint>
#include <string>

 // Define the shader data types
enum class ShaderDataType
{
	None = 0,
	Float,
	Float2,
	Float3,
	Float4,
	Mat3,
	Mat4,
	Int,
	Int2,
	Int3,
	Int4,
	Bool
};

// A function for getting the size of the given shader data type
static uint32_t ShaderDataTypeSize(ShaderDataType type);

// Define a struct for a buffer element
struct BufferElement
{
	std::string name;
	ShaderDataType type;
	uint32_t size;
	uint32_t offset;
	bool normalised;

	BufferElement(const std::string& name, ShaderDataType type, bool normalised = false);
	uint32_t getComponentCount() const;
};
