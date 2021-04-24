#include "pch.h"

#include "Core/Window.h"

#ifdef PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Basil
{
	Unique<Window> Window::create(const WindowProps& props)
	{
		#ifdef PLATFORM_WINDOWS
			return makeUnique<WindowsWindow>(props);
		#else
			ASSERT(false, "Unknown platform.");
			return nullptr;
		#endif
	}
}