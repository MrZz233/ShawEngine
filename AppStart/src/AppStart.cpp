#include <Engine.h>
#include <Engine/Core/EntryPoint.h>

#include "AppStart2D.h"
#include "ExampleLayer.h"
class AppStart :public ShawEngine::Application {
public:
	AppStart(ShawEngine::ApplicationCommandLineArgs args) {
		//PushLayer(new ExampleLayer());
		PushLayer(new AppStart2D());
	}

	~AppStart() {

	}

};

ShawEngine::Application* ShawEngine::CreateApplication(ShawEngine::ApplicationCommandLineArgs args)
{
	return new AppStart(args);
}