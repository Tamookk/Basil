#pragma once

#include "Scene/Entity.h"
#include "Scripting/ScriptGlue.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>

namespace Basil
{
	class ScriptClass
	{
		public:
			ScriptClass() = default;
			ScriptClass(const std::string& classNamespace, const std::string& className);

			MonoObject* instantiate();
			MonoMethod* getMethod(const std::string& name, int parameterCount);
			MonoObject* invokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

		private:
			std::string classNamespace;
			std::string className;

			MonoClass* monoClass = nullptr;
	};

	class ScriptInstance
	{
		public:
			ScriptInstance(Shared<ScriptClass> scriptClass, Entity entity);
			void invokeOnCreate();
			void invokeOnUpdate(float timeStep);

		private:
			Shared<ScriptClass> scriptClass;

			MonoObject* instance;
			MonoMethod* constructor;
			MonoMethod* onCreateMethod;
			MonoMethod* onUpdateMethod;
	};

	class ScriptEngine
	{
	public:
		static void init();
		static void shutdown();
		static void loadAssembly(const std::filesystem::path& filePath);

		static void onRuntimeStart(Scene* scene);
		static void onRuntimeStop();

		static bool entityClassExists(const std::string& fullClassName);
		static void onCreateEntity(Entity entity);
		static void onUpdateEntity(Entity entity, Timestep timeStep);

		static Scene* getSceneContext();
		static std::unordered_map<std::string, Shared<ScriptClass>> getEntityClasses();

		static _MonoImage* getCoreAssemblyImage();
	private:
		static void initMono();
		static void shutdownMono();
		static MonoObject* instantiateClass(MonoClass* monoClass);
		static void loadAssemblyClasses(MonoAssembly* assembly);

		friend class ScriptClass;
		friend class ScriptGlue;
	};
}
