/*
 * This header file is used by the engine, and games 
 * created using it, to determine whether to dllexport or dllimport
 */

#pragma once

#ifdef PLATFORM_WINDOWS
	#ifdef BUILD_DLL
		#define BASIL_API __declspec(dllexport)
	#else
		#define BASIL_API __declspec(dllimport)
	#endif
#else
	#error Basil only supports Windows
#endif

