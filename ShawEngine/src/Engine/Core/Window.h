#pragma once

#include <sstream>
#include "Engine/Core/Base.h"
#include "Engine/Events/Event.h"

namespace ShawEngine {

	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		//默认窗口字段
		WindowProps(const std::string& title = "ShawEngine",
					uint32_t width = 1280,
					uint32_t height = 720)
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

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		//窗口属性
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		//函数声明，不同的平台需要实现各自的Create()，目前有Windows
		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};

}