#include "pch.h"

#include <fstream>

#include "Core/Timer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace Basil
{
	namespace Utils
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

		// GL Shader stage to ShaderC type
		static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:
					return shaderc_glsl_vertex_shader;
				case GL_FRAGMENT_SHADER:
					return shaderc_glsl_fragment_shader;
			}

			ASSERT(false, "");
			return (shaderc_shader_kind)0;
		}

		// GL Shader stage to string
		static const char* GLShaderStageToString(GLenum stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:
					return "GL_VERTEX_SHADER";
				case GL_FRAGMENT_SHADER:
					return "GL_FRAGMENT_SHADER";
			}

			ASSERT(false, "");
			return nullptr;
		}

		// Return the cache directory
		static const char* getCacheDirectory()
		{
			return "assets/cache/shader/opengl";
		}

		// Create the cache directory
		static void createCacheDirectoryIfNeeded()
		{
			std::string cacheDirectory = getCacheDirectory();

			if (!std::filesystem::exists(cacheDirectory))
				std::filesystem::create_directories(cacheDirectory);
		}

		// Return GL shader stage cached file extension (OpenGL)
		static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:
					return ".cached_opengl.vert";
				case GL_FRAGMENT_SHADER:
					return ".cached_opengl.frag";
			}

			ASSERT(false, "");
			return "";
		}

		// Return GL shader stage cached file extension (Vulkan)
		static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:
					return ".cached_vulkan.vert";
				case GL_FRAGMENT_SHADER:
					return ".cached_vulkan.frag";
			}

			ASSERT(false, "");
			return "";
		}
	}

	// Constructor
	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		PROFILE_FUNCTION();

		// Initialise variables
		rendererID = 0;
		this->filePath = filePath;

		// Create cache directory if needed
		Utils::createCacheDirectoryIfNeeded();

		// Read the shader source from the file
		std::string shaderSource = readFile(filePath);

		// Create an unordered map of the shader sources
		std::unordered_map<GLenum, std::string> shaderSources = preProcess(shaderSource);

		// Compile the shader sources
		{
			Timer timer;
			compileOrGetVulkanBinaries(shaderSources);
			compileOrGetOpenGLBinaries();
			createProgram();
			LOG_WARN("Shader creation took {0} ms", timer.elapsedMillis());
		}

		// Extract name from file path
		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind('.');
		auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
		name = filePath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
	{
		PROFILE_FUNCTION();

		// Initialise variables
		rendererID = 0;
		this->name = name;

		// Create an unordered map of the shader source strings
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexShaderSource;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentShaderSource;

		// Compile the shader sources
		compileOrGetVulkanBinaries(shaderSources);
		compileOrGetOpenGLBinaries();
		createProgram();
	}

	// Destructor
	OpenGLShader::~OpenGLShader()
	{
		PROFILE_FUNCTION();

		glDeleteProgram(rendererID);
	}

	// Bind the shader
	void OpenGLShader::bind() const
	{
		PROFILE_FUNCTION();

		glUseProgram(rendererID);
	}

	// Unbind the shader
	void OpenGLShader::unbind() const
	{
		PROFILE_FUNCTION();

		glUseProgram(0);
	}

	// Get the name of the shader
	const std::string& OpenGLShader::getName() const
	{
		return name;
	}

	// Set an integer uniform
	void OpenGLShader::setInt(const std::string& name, int value)
	{
		PROFILE_FUNCTION();

		uploadUniformInt(name, value);
	}

	// Set an integer array uniform
	void OpenGLShader::setIntArray(const std::string& name, int* values, uint32_t count)
	{
		PROFILE_FUNCTION();

		uploadUniformIntArray(name, values, count);
	}

	// Set a float uniform
	void OpenGLShader::setFloat(const std::string& name, float value)
	{
		PROFILE_FUNCTION();

		uploadUniformFloat(name, value);
	}

	// Set a float 3 uniform
	void OpenGLShader::setFloat3(const std::string& name, const glm::vec3& value)
	{
		PROFILE_FUNCTION();

		uploadUniformFloat3(name, value);
	}

	// Set a float 4 uniform
	void OpenGLShader::setFloat4(const std::string& name, const glm::vec4& value)
	{
		PROFILE_FUNCTION();

		uploadUniformFloat4(name, value);
	}

	// Set a mat 4 uniform
	void OpenGLShader::setMat4(const std::string& name, const glm::mat4& matrix)
	{
		PROFILE_FUNCTION();

		uploadUniformMat4(name, matrix);
	}

	// Upload an integer uniform
	void OpenGLShader::uploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniform1i(location, value);
	}

	// Upload an integer array uniform
	void OpenGLShader::uploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniform1iv(location, count, values);
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
		PROFILE_FUNCTION();

		// Open the file
		std::string result;
		std::ifstream file(filePath, std::ios::in | std::ios::binary);

		// Check the file could be opened
		if (file)
		{
			// Go to the end of the file, get the size, resize the result string to be the file length
			file.seekg(0, std::ios::end);

			// Check that the file can be read
			size_t size = file.tellg();
			if (size != -1)
			{
				result.resize(file.tellg());
				file.seekg(0, std::ios::beg);
				file.read(&result[0], result.size());
				file.close();
			}
			else
			{
				LOG_ERROR("Could not read from file '{0}'", filePath);
			}
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
		PROFILE_FUNCTION();

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
			ASSERT(Utils::shaderTypeFromString(type), "Invalid shader type specified!");

			// Get the position of the next type
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(delimiter, nextLinePos);

			// Add the shader source to the unordered map
			shaderSources[Utils::shaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	// Compile Vulkan shader
	void OpenGLShader::compileOrGetVulkanBinaries(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		PROFILE_FUNCTION();

		// Create a program
		GLuint program = glCreateProgram();

		// Create shaderc compiler and set properties
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		// Get cache directory
		std::filesystem::path cacheDirectory = Utils::getCacheDirectory();

		// Read shader data and compile
		auto& shaderData = VulkanSPIRV;
		shaderData.clear();
		for (auto&& [stage, source] : shaderSources)
		{
			std::filesystem::path shaderFilePath = filePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), filePath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					LOG_ERROR(module.GetErrorMessage());
					ASSERT(false, "");
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto&& [stage, data] : shaderData)
			reflect(stage, data);
	}

	// Compile OpenGL shader
	void OpenGLShader::compileOrGetOpenGLBinaries()
	{
		// Get sahder data
		auto& shaderData = OpenGLSPIRV;

		// Create shaderc compiler and set properties
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = false;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		// Get cache directory
		std::filesystem::path cacheDirectory = Utils::getCacheDirectory();

		// Read shader data and compile
		shaderData.clear();
		OpenGLSourceCode.clear();
		for (auto&& [stage, spirv] : VulkanSPIRV)
		{
			std::filesystem::path shaderFilePath = filePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				OpenGLSourceCode[stage] = glslCompiler.compile();
				auto& source = OpenGLSourceCode[stage];

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), filePath.c_str());
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					LOG_ERROR(module.GetErrorMessage());
					ASSERT(false, "");
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}

	// Create shader program
	void OpenGLShader::createProgram()
	{
		GLuint program = glCreateProgram();

		// Create shaders
		std::vector<GLuint> shaderIDs;
		for (auto&& [stage, spirv] : OpenGLSPIRV)
		{
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		// Link program
		glLinkProgram(program);

		// Delete shaders and program if linking failed
		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			LOG_ERROR("Shader linking failed ({0}):\n{1}", filePath, infoLog.data());
			glDeleteProgram(program);

			for (auto id : shaderIDs)
				glDeleteShader(id);
		}

		// Linking succeeded, detach and delete shaders
		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		rendererID = program;
	}

	// Reflect?
	void OpenGLShader::reflect(GLenum stage, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		LOG_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(stage), filePath);
		LOG_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
		LOG_TRACE("    {0} resources", resources.sampled_images.size());

		LOG_TRACE("Uniform buffers:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = bufferType.member_types.size();

			LOG_TRACE("  {0}", resource.name);
			LOG_TRACE("    Size = {0}", bufferSize);
			LOG_TRACE("    Binding = {0}", binding);
			LOG_TRACE("    Members = {0}", memberCount);
		}
	}
}