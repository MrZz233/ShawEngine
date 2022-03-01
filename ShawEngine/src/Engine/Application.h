#pragma once
#include "Core.h"
#include "Window.h"

namespace ShawEngine {
	class SE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();		
	private:
		std::unique_ptr<Window> m_Windows;
		bool m_Runing;
	};


	extern Application* CreateApplication();
}
