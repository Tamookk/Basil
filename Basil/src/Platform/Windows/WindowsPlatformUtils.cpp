#include "pch.h"
#include "Core/Application.h"
#include "Utils/PlatfomUtils.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Basil
{
	// Open file dialog
	std::string FileDialogs::openFile(const char* filter)
	{
		// Create struct
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		
		// Zero out struct
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		
		// Set struct parameters
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::get().getWindow().getNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		// Attempt to open the file
		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}

		return std::string();
	}

	// Close file dialog
	std::string FileDialogs::saveFile(const char* filter)
	{
		// Create struct
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };

		// Zero out struct
		ZeroMemory(&ofn, sizeof(OPENFILENAME));

		// Set struct parameters
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::get().getWindow().getNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrDefExt = std::strchr(filter, '\0') + 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		// Attempt to open the file
		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}

		return std::string();
	}
}