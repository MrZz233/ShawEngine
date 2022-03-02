#pragma once
#include "Engine/Layer.h"
#include "Engine/Events/Event_Key.h"
#include "Engine/Events/Event_Mouse.h"
#include "Engine/Events/Event_Application.h"

namespace ShawEngine {
	class SE_API ImGuiLayer :public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& event) override;
		
	private:
		bool OnMouseButtonPressed(MouseButtonPressedEvent&);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent&);
		bool OnMouseMoved(MouseMovedEvent&);
		bool OnMouseSrolled(MouseScrolledEvent&);
		bool OnKeyPressed(KeyPressedEvent&);
		bool OnKeyReleased(KeyReleasedEvent&);
		bool OnKeyTyped(KeyTypedEvent&);
		bool OnWindowResized(WindowResizeEvent&);

		float m_Time = 0.0f;
		bool needRender = true;
	};
}