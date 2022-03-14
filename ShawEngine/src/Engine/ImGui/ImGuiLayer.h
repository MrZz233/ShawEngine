#pragma once
#include "Engine/Core/Layer.h"
#include "Engine/Events/Event_Key.h"
#include "Engine/Events/Event_Mouse.h"
#include "Engine/Events/Event_Application.h"

namespace ShawEngine {
	class ImGuiLayer :public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();
		void BlockEvents(bool block) { m_BlockEvents = block; }
		void SetDarkThemeColors();
	private:
		//float m_Time = 0.0f;
		bool m_BlockEvents = true;
	};
}