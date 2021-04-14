#include "pch.h"

#include <fstream>

#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Basil
{
	// Return shader type based on #type definiton in shader file
	static GLenum shaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		else if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		ASSERT(false, "Unknown shader type!");
		return 0;
	}

	// Constructor
	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		// Initialise rendererID
		rendererID = 0;

		// Read the shader source from the file
		std::string shaderSource = readFile(filePath);

		// Create an unordered map of the shader sources
		std::unordered_map<GLenum, std::string> shaderSources = preProcess(shaderSource);

		// Compile the shader sources
		compile(shaderSources);

		// Extract name from file path
		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind('.');
		auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
		name = filePath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
	{
		// Initialise variables
		rendererID = 0;
		this->name = name;

		// Create an unordered map of the shader source strings
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexShaderSource;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentShaderSource;

		// Compile the shader sources
		compile(shaderSources);
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

	// Get the name of the shader
	const std::string& OpenGLShader::getName() const
	{
		return name;
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

	// Read a shader file's contents
	std::string OpenGLShader::readFile(const std::string& filePath)
	{
		// Open the file
		std::string result;
		std::ifstream file(filePath, std::ios::in | std::ios::binary);

		// Check the file could be opened
		if (file)
		{
			// Go to the end of the file, get the size, resize the result string to be the file length
			file.seekg(0, std::ios::end);
			result.resize(file.tellg());
			file.seekg(0, std::ios::beg);
			file.read(&result[0], result.size());
			file.close();
		}
		else
		{
			LOG_ERROR("Could not open file '{0}'!", filePath);
		}

		// Return the file's contents as a string
		return result;
	}

	// Split shader file's contents into seperate shader types
	std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string& source)
	{
		// Create a map to store the shader sources
		std::unordered_map<GLenum, std::string> shaderSources;

		// Search for the shader type delimiter
		const char* delimiter = "#type";
		size_t delimiterLength = strlen(delimiter);
		size_t pos = source.find(delimiter, 0);

		// While there is a shader source to be read
		while (pos != std::string::npos)
		{
			// Get the position of the end of the #type line (no carriage returns in Unix files, need to edit
			// this when developing Basil for Unix-based OSes)
			size_t eol = source.find_first_of("\r\n", pos);

			// Ensure that the end of the line is not the end of the file
			ASSERT(eol != std::string::npos, "Syntax error!");

			// Get the type of the shader source
			size_t begin = pos + delimiterLength + 1;
			std::string type = source.substr(begin, eol - begin);
			ASSERT(ShaderTypeFromString(type), "Invalid shader type specified!");

			// Get the position of the next type
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(delimiter, nextLinePos);

			// Add the shader source to the unordered map
			shaderSources[shaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	// Compile the shader
	void OpenGLShader::compile(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		// Create a shader program
		GLuint program = glCreateProgram();

		// Assert that there are only 2 shader sources
		ASSERT(shaderSources.size() <= 2, "Only 2 shaders currently supported!");

		// Create an array of shader IDs
		std::array<GLenum, 2> shaderIDs;
		int shaderIDIndex = 0;

		// Iterate through the shader sources to link and compile each one
		for (auto& kv : shaderSources)
		{
			// Get type and source
			GLenum type = kv.first;
			const std::string& shaderSource = kv.second;

			// Create the shader
			GLuint shader = glCreateShader(type);

			// Send the shader source code to OpenGL
			const GLchar* source = shaderSource.c_str();
			glShaderSource(shader, 1, &source, 0);

			// Compile the shader
			glCompileShader(shader);

			// Check that the shader compiled
			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				// Get the error message and length
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> infoLog(maxLength);

				// Delete the shader
				glDeleteShader(shader);

				// Log that the shader failed to compile
				LOG_ERROR("{0}", infoLog.data());
				ASSERT(false, "Vertex shader compilation failure.");
				return;
			}

			// Attach the shader to the program and add its ID to the list
			glAttachShader(program, shader);
			shaderIDs[shaderIDIndex++] = shader;
		}

		// Set renderer ID variable
		rendererID = program;

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
			
			for(auto shader : shaderIDs)
				glDeleteShader(shader);

			LOG_ERROR("{0}", infoLog.data());
			ASSERT(false, "Shader link failure.");
			return;
		}

		// Detach shaders
		for (auto shader : shaderIDs)
			glDetachShader(program, shader);
	}
}