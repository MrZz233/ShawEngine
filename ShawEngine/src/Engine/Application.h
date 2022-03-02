#pragma once
#include "Core.h"
#include "Window.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/Event_Application.h"
#include "Engine/LayerStack.h"
#include "Engine/ImGui/ImGuiLayer.h"

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

		inline Window& GetWindow() { return *m_Windows; }

		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnClosed(WindowCloseEvent&);
		std::unique_ptr<Window> m_Windows;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running;
		LayerStack m_LayerStack;

		static Application* s_Instance;
	};


	extern Application* CreateApplication();
}
