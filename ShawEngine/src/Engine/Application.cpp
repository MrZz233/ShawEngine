#include "sepch.h"
#include "Application.h"
#include "Engine/Log.h"
#include "glad/glad.h"

namespace ShawEngine {
#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() {

		SE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Windows = std::unique_ptr<Window>(Window::Create());
		m_Windows->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		unsigned int id;
		glGenVertexArrays(1, &id);
	}
	Application::~Application() {

	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverLay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
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
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
			m_Windows->OnUpdate();
		}
	}
	
}