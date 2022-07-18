#pragma once

#include "Scripting/ScriptGlue.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>

namespace Basil
{
	class ScriptEngine
	{
		public:
			static void init();
			static void shutdown();
			static void loadAssembly(const std::filesystem::path& filePath);
		private:
			static void initMono();
			static void shutdownMono();
			static MonoObject* instantiateClass(MonoClass* monoClass);

			friend class ScriptClass;
	};

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
}
