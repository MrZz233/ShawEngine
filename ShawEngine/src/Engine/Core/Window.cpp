#include "sepch.h"
#include "Engine/Core/Window.h"

#ifdef SE_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace ShawEngine
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef SE_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		SE_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}