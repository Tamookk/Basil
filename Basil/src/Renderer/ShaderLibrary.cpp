#include "pch.h"
#include "Renderer/ShaderLibrary.h"

namespace Basil
{
	// Add a shader to the library
	void ShaderLibrary::add(const Shared<Shader>& shader)
	{
		const std::string& name = shader->getName();
		add(name, shader);
	}

	void ShaderLibrary::add(const std::string& name, const Shared<Shader>& shader)
	{
		ASSERT(!exists(name), "Shader already exists!");
		shaders[name] = shader;
	}

	// Load a shader
	Shared<Shader> ShaderLibrary::load(const std::string& filePath)
	{
		Shared<Shader> shader = Shader::create(filePath);
		add(shader);
		return shader;
	}

	Shared<Shader> ShaderLibrary::load(const std::string& name, const std::string& filePath)
	{
		Shared<Shader> shader = Shader::create(filePath);
		add(name, shader);
		return shader;
	}

	// Get a shader
	Shared<Shader> ShaderLibrary::get(const std::string& name)
	{
		ASSERT(exists(name), "Shader not found!");
		return shaders[name];
	}

	// Check if a shader exists	
	bool ShaderLibrary::exists(const std::string& name)
	{
		return shaders.find(name) != shaders.end();
	}
}