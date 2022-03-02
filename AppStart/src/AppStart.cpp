#include <Engine.h>
#include "imgui/imgui.h"

class ExampleLayer :public ShawEngine::Layer {
public:
	ExampleLayer() :Layer("Example") {

	}

	void OnUpdate() override {
		//轮询访问是否按下了某键
		if (ShawEngine::Input::IsKeyPressed(SE_KEY_TAB))
			SE_CLIENT_TRACE("Tab key is pressed (poll)!");
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(ShawEngine::Event& event) override {
		if (event.GetEventType() == ShawEngine::EventType::KeyPressed)
		{
			ShawEngine::KeyPressedEvent& e = (ShawEngine::KeyPressedEvent&)event;
			if (e.GetKeyCode() == SE_KEY_TAB)
				SE_CLIENT_TRACE("Tab key is pressed (event)!");
			SE_CLIENT_TRACE("{0}", (char)e.GetKeyCode());
		}
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