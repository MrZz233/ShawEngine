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

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			SE_CORE_ASSERT(index < Count);
			return Args[index];
		}
	};

	class Application
	{
	public:
		Application(const std::string& name = "ShawEngine App", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();

		void OnEvent(Event&);

		void PushLayer(Layer* layer);
		void PushOverLay(Layer* layer);

		Window& GetWindow() { return *m_Windows; }
		void Close();
		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		static Application& Get() { return *s_Instance; }
		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }
		bool OnWindowClose(WindowCloseEvent& e);
	private:
		void Run();
		bool OnClosed(WindowCloseEvent&);
		bool OnWindowResize(WindowResizeEvent& e);
		ApplicationCommandLineArgs m_CommandLineArgs;
		Scope<Window> m_Windows;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	//用户定义
	Application* CreateApplication(ApplicationCommandLineArgs args);
}
