#include <iostream>
#include "Engine.h"

class ExampleLayer :public ShawEngine::Layer {
public:
	ExampleLayer() :Layer("Example") {

	}

	void OnUpdate() override {
		SE_CLIENT_INFO("ExampleLayer Update..");
	}

	void OnEvent(ShawEngine::Event& e) override {
		SE_CLIENT_INFO("{0}:{1}","ExampleLayer", e);
	}

};


class AppStart :public ShawEngine::Application {
public:
	AppStart() {
		PushLayer(new ExampleLayer());
	}

	~AppStart() {

	}

};

ShawEngine::Application* ShawEngine::CreateApplication() {
	return new AppStart();
}