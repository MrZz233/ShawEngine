#pragma once
#include "Engine/Core/Core.h"
#include "Engine/Core/Window.h"
#include "Engine/Core/LayerStack.h"
#include "Engine/Core/Timestep.h"

#include "Engine/Events/Event.h"
#include "Engine/Events/Event_Application.h"

#include "Engine/ImGui/ImGuiLayer.h"

namespace ShawEngine {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();	
		void OnEvent(Event&);

		void PushLayer(Layer* layer);
		void PushOverLay(Layer* layer);

		inline Window& GetWindow() { return *m_Windows; }

		inline static Application& Get() { return *s_Instance; }
		bool OnWindowClose(WindowCloseEvent& e);
	private:
		bool OnClosed(WindowCloseEvent&);
		bool OnWindowResize(WindowResizeEvent& e);
		std::unique_ptr<Window> m_Windows;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

		static Application* s_Instance;
	};

	//用户定义
	extern Application* CreateApplication();
}
