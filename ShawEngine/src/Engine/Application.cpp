#include "sepch.h"
#include "Application.h"
#include "Engine/Log.h"
#include "GLFW/glfw3.h"

namespace ShawEngine {
#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)

	Application::Application() {
		m_Windows = std::unique_ptr<Window>(Window::Create());
		m_Windows->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}
	Application::~Application() {

	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispacther(e);
		//如果e是窗口关闭事件，执行Onclosed()
		dispacther.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnClosed));
		SE_CORE_TRACE("{0}",e);
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
			m_Windows->OnUpdate();
		}
	}
	
}