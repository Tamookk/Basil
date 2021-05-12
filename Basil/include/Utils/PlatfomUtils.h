/*
 * This header file declares utilities implemented for each platform.
 */
#pragma once

#include <string>

namespace Basil
{
	class FileDialogs
	{
	public:
		static std::string openFile(const char* filter);
		static std::string saveFile(const char* filter);
	};
}
