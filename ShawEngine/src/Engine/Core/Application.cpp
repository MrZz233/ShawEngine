#include "sepch.h"
#include "Engine/Core/Application.h"
#include "Engine/Core/Log.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Core/Input.h"
#include <glfw/glfw3.h>

namespace ShawEngine {

	Application* Application::s_Instance = nullptr;

	Application::Application(){
		SE_PROFILE_FUNCTION();
		SE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Windows = Window::Create();
		m_Windows->SetEventCallback(SE_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
		m_ImGuiLayer = new ImGuiLayer();
		PushOverLay(m_ImGuiLayer);
	}

	Application::~Application() {
		SE_PROFILE_FUNCTION();
		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer) {
		SE_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverLay(Layer* layer) {
		SE_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& e) {
		SE_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		//如果e是窗口关闭事件，执行Onclosed()
		dispatcher.Dispatch<WindowCloseEvent>(SE_BIND_EVENT_FN(Application::OnClosed));
		dispatcher.Dispatch<WindowResizeEvent>(SE_BIND_EVENT_FN(Application::OnWindowResize));
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) 
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	bool Application::OnClosed(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	void Application::Run() {
		SE_PROFILE_FUNCTION();
		while (m_Running) {
			SE_PROFILE_SCOPE("RunLoop");
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					SE_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}
				//渲染ImGui
				m_ImGuiLayer->Begin();
				{
					SE_PROFILE_SCOPE("LayerStack OnImGuiRender");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}
			m_Windows->OnUpdate();
		}
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		SE_PROFILE_FUNCTION();
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}