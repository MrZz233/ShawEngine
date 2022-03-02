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

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};
}