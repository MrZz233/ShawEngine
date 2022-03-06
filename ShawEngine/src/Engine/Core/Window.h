#pragma once

#include "sepch.h"
#include "Engine/Core/Core.h"
#include "Engine/Events/Event.h"

namespace ShawEngine {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		//默认窗口字段
		WindowProps(const std::string& title = "ShawEngine",
			        unsigned int width = 1280,
			        unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	//窗口基类
	class Window
	{
	public:
		//回调
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		//窗口属性
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		//函数声明，不同的平台需要实现各自的Create()，目前有Windows
		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};

}