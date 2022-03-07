#pragma once

#include "Engine/Core/Base.h"

namespace ShawEngine {

	class Input
	{
	
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		//用户使用，屏蔽具体平台
		static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

		static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
		static Scope<Input> Create();

	protected:
		Input() = default;
		//由不同的子类实现
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Scope<Input> s_Instance;
	};

}