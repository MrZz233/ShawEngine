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

		//Ĭ�ϴ����ֶ�
		WindowProps(const std::string& title = "ShawEngine",
			        unsigned int width = 1280,
			        unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	//���ڻ���
	class Window
	{
	public:
		//�ص�
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		//��������
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		//������������ͬ��ƽ̨��Ҫʵ�ָ��Ե�Create()��Ŀǰ��Windows
		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};

}