#include "sepch.h"
#include "Application.h"
#include "Engine/Events/Event_Application.h"
#include "Engine/Log.h"
#include "GLFW/glfw3.h"

namespace ShawEngine {
	Application::Application() {
		m_Windows = std::unique_ptr<Window>(Window::Create());
	}
	Application::~Application() {

	}
	void Application::Run() {
		while (m_Runing) {
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Windows->OnUpdate();
		}
	}
	
}