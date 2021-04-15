/*
 * 
 */
#pragma once

#include <unordered_map>

#include "Core/Core.h"
#include "Renderer/Shader.h"

namespace Basil
{
	class ShaderLibrary
	{
		public:
			void add(const Shared<Shader>& shader);
			void add(const std::string& name, const Shared<Shader>& shader);
			Shared<Shader> load(const std::string& filePath);
			Shared<Shader> load(const std::string& name, const std::string& filePath);
			Shared<Shader> get(const std::string& name);
			bool exists(const std::string& name);
		private:
			std::unordered_map<std::string, Shared<Shader>> shaders;
	};
}