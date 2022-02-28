#include "Application.h"
#include "Engine/Events/Event_Application.h"
#include "Engine/Log.h"

namespace ShawEngine {
	Application::Application() {

	}
	Application::~Application() {

	}
	void Application::Run() {
		WindowResizeEvent event_window_resize(1280, 720);
		SE_CLIENT_TRACE(event_window_resize);

		while (true) {

		}
	}
	
}