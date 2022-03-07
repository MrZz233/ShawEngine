#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Core/Window.h"
#include "Engine/Core/LayerStack.h"
#include "Engine/Core/Timestep.h"

#include "Engine/Events/Event.h"
#include "Engine/Events/Event_Application.h"

#include "Engine/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace ShawEngine {
	class Application
	{
	public:
		Application(const std::string& name = "ShawEngine App");
		virtual ~Application();

		void OnEvent(Event&);

		void PushLayer(Layer* layer);
		void PushOverLay(Layer* layer);

		Window& GetWindow() { return *m_Windows; }
		void Close();
		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		static Application& Get() { return *s_Instance; }
		bool OnWindowClose(WindowCloseEvent& e);
	private:
		void Run();
		bool OnClosed(WindowCloseEvent&);
		bool OnWindowResize(WindowResizeEvent& e);
		std::unique_ptr<Window> m_Windows;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	//用户定义
	extern Application* CreateApplication();
}
