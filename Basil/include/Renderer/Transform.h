/*
 * Declares a struct for storing and changing properties of an object to be rendered.
 */
#pragma once

struct Position
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

struct Scale
{
	float x = 1.0f;
	float y = 1.0f;
	float z = 1.0f;
};

namespace Basil
{
	struct Transform
	{
		Position position;
		float rotation = 0.0f;
		Scale scale;
	};
}