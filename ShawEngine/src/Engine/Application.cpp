#include "sepch.h"
#include "Application.h"
#include "Engine/Log.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Input.h"
#include <glfw/glfw3.h>

namespace ShawEngine {
#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(){

		SE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Windows = std::unique_ptr<Window>(Window::Create());
		m_Windows->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
		m_ImGuiLayer = new ImGuiLayer();
		PushOverLay(m_ImGuiLayer);
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverLay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispacther(e);
		//如果e是窗口关闭事件，执行Onclosed()
		dispacther.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnClosed));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Application::OnClosed(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	void Application::Run() {
		std::shared_ptr<int> p(new int);
		while (m_Running) {
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			//渲染ImGui
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Windows->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}