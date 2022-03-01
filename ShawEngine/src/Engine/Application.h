#pragma once
#include "Core.h"
#include "Window.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/Event_Application.h"
#include "Engine/LayerStack.h"

namespace ShawEngine {
	class SE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();	
		void OnEvent(Event&);

		void PushLayer(Layer* layer);
		void PushOverLay(Layer* layer);
	private:
		bool OnClosed(WindowCloseEvent&);
		std::unique_ptr<Window> m_Windows;
		bool m_Running;
		LayerStack m_LayerStack;
	};


	extern Application* CreateApplication();
}
