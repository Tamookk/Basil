#include "pch.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Basil
{
	// Constructor
	OpenGLShader::OpenGLShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to OpenGL
		const GLchar* source = vertexShaderSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile ths vertex shader
		glCompileShader(vertexShader);

		// Check that the shader compiled
		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			// Get the error message and length
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);

			// Delete the shader
			glDeleteShader(vertexShader);

			// Log that the shader failed to compile
			LOG_ERROR("{0}", infoLog.data());
			ASSERT(false, "Vertex shader compilation failure.");
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the vertex shader source code to OpenGL
		source = fragmentShaderSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile ths vertex shader
		glCompileShader(fragmentShader);

		// Check that the shader compiled
		isCompiled = 0;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			// Get the error message and length
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);

			// Delete the shaders
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Log that the shader failed to compile
			LOG_ERROR("{0}", infoLog.data());
			ASSERT(false, "Fragment shader compilation failure.");
			return;
		}

		// Create a shader program
		rendererID = glCreateProgram();
		GLuint program = rendererID;

		// Attach shaders to the program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		// Link the program
		glLinkProgram(program);

		// Check that the program linked correctly
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			glDeleteProgram(program);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			LOG_ERROR("{0}", infoLog.data());
			ASSERT(false, "Shader link failure.");
			return;
		}

		// Detach shaders
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	// Destructor
	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(rendererID);
	}

	// Bind the shader
	void OpenGLShader::bind() const
	{
		glUseProgram(rendererID);
	}

	// Unbind the shader
	void OpenGLShader::unbind() const
	{
		glUseProgram(0);
	}

	// Upload an integer uniform
	void OpenGLShader::uploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniform1i(location, value);
	}

	// Upload a float uniform
	void OpenGLShader::uploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniform1f(location, value);
	}

	// Upload a vec2 of floats uniform
	void OpenGLShader::uploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	// Upload a vec3 of floats uniform
	void OpenGLShader::uploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	// Upload a vec4 of floats uniform
	void OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	// Upload a 3x3 matrix uniform
	void OpenGLShader::uploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	// Upload a 4x4 matrix uniform
	void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}