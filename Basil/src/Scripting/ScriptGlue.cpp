#include "pch.h"
#include "Scripting/ScriptGlue.h"

#include <mono/metadata/object.h>

namespace Basil
{
#define ADD_INTERNAL_CALL(Name) mono_add_internal_call("Basil.InternalCalls::" #Name, Name)

	// Logging from C# - cout string followed by parameter
	static void NativeLog(MonoString* string, int parameter)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string message(cStr);
		mono_free(cStr);
		std::cout << message << ", " << parameter << std::endl;
	}

	static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
	{
		// glm::vec3 shouldn't be needed but macros in Log.h are being stupid
		LOG_WARN("Value: {0}", glm::to_string(*parameter));
		*outResult = glm::normalize(*parameter);
	}

	static float NativeLog_VectorDot(glm::vec3* parameter)
	{
		LOG_WARN("Value: {0}", glm::to_string(*parameter));
		return glm::dot(*parameter, *parameter);
	}

	void ScriptGlue::registerFunctions()
	{
		ADD_INTERNAL_CALL(NativeLog);
		ADD_INTERNAL_CALL(NativeLog_Vector);
		ADD_INTERNAL_CALL(NativeLog_VectorDot);
	}
}