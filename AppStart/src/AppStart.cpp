#include <iostream>
#include "Engine.h"

class AppStart :public ShawEngine::Application {
public:
	AppStart() {

	}

	~AppStart() {

	}

};

ShawEngine::Application* ShawEngine::CreateApplication() {
	return new AppStart();
}