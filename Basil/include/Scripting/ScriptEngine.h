#pragma once

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
		private:
			static void initMono();
			static void shutdownMono();
	};
}
