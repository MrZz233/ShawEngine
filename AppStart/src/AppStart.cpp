#include <Engine.h>
#include <Engine/Core/EntryPoint.h>

#include "AppStart2D.h"
#include "ExampleLayer.h"
class AppStart :public ShawEngine::Application {
public:
	AppStart() {
		//PushLayer(new ExampleLayer());
		PushLayer(new AppStart2D());
	}

	~AppStart() {

	}

};

ShawEngine::Application* ShawEngine::CreateApplication() {
	return new AppStart();
}