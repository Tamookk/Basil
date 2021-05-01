/*
 * This header file is used by the engine, and games 
 * created using it, to make various definitions and
 * declare various macros.
 */
#pragma once

#include <memory>

#ifdef DEBUG
	#ifdef PLATFORM_WINDOWS
		#define DEBUG_BREAK() __debugbreak()
	#else
		#error "Platform doesn't support debugbreak yet."
	#endif
	#define ENABLE_ASSERTS
#else
	#define DEBUG_BREAK()
#endif

#ifdef ENABLE_ASSERTS
	#define ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); DEBUG_BREAK(); } }
	#define ASSERT_CLIENT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); DEBUG_BREAK(); } }
#else
	#define ASSERT(x, ...)
	#define ASSERT_CLIENT(x, ...)
#endif

#define BIND_EVENT(x) std::bind(&x, this, std::placeholders::_1)

namespace Basil
{
	template <typename T>
	using Unique = std::unique_ptr<T>;

	template <typename T, typename ... Args>
	constexpr Unique<T> makeUnique(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
	

	template <typename T>
	using Shared = std::shared_ptr<T>;

	template <typename T, typename ... Args>
	constexpr Shared<T> makeShared(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}