/*
 * This header declares a class for serializing scenes.
 */
#pragma once

#include "Scene/Scene.h"

namespace Basil
{
	class SceneSerializer
	{
		public:
			SceneSerializer(const Shared<Scene>& scene);
			void serialize(const std::string& filePath);
			void serializeRuntime(const std::string& filePath);
			bool deserialize(const std::string& filePath);
			bool deserializeRuntime(const std::string& filePath);
		private:
			Shared<Scene> scene;
	};
}