/*
 * This header file is used by the engine, and games 
 * created using it, to determine whether to dllexport or dllimport
 */

#pragma once

#ifdef PLATFORM_WINDOWS
	#ifdef DYNAMIC_LINK
		#ifdef BUILD_DLL
			#define BASIL_API __declspec(dllexport)
		#else
			#define BASIL_API __declspec(dllimport)
		#endif
	#else
		#define BASIL_API
	#endif
#else
	#error Basil only supports Windows
#endif

#ifdef ENABLE_ASSERTS
	#define ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define ASSERT_CLIENT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define ASSERT(x, ...)
	#define ASSERT_CLIENT(x, ...)
#endif

#define BIND_EVENT(x) std::bind(&x, this, std::placeholders::_1)

namespace Basil
{
	template <typename T>
	using Unique = std::unique_ptr<T>;
	template <typename T>
	using Shared = std::shared_ptr<T>;
}