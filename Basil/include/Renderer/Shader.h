/*
 * This header file declares a class for creating an OpenGL shader.
 */
#pragma once

#include <string>

namespace Basil
{
	class Shader
	{
		public:
			virtual ~Shader() {}
			virtual void bind() const = 0;
			virtual void unbind() const = 0;
			static Shader* create(const std::string& filePath);
			static Shader* create(const std::string& vertexSource, const std::string& fragmentSource);
	};
}
