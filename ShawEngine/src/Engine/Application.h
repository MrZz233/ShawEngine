#pragma once
#include <iostream>
#include "Core.h"

namespace ShawEngine {
	class SE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();		
	};

	extern Application* CreateApplication();
}
