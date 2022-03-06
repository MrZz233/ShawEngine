#pragma once
#include "Engine/Events/Event.h"

//Window和App相关的类
namespace ShawEngine {
	
	//调整窗口大小事件，继承自Event
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }
		
		//重写TosTring()，打印事件类型，以及详情
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		//通过宏快速实现事件枚举和分类相关
		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};

	//窗口关闭事件，继承自Event
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		//通过宏快速实现事件枚举和分类相关
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	//程序点击事件，继承自Event
	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		//通过宏快速实现事件枚举和分类相关
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	//程序主循环事件，继承自Event
	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		//通过宏快速实现事件枚举和分类相关
		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	//程序渲染事件，继承自Event
	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		//通过宏快速实现事件枚举和分类相关
		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}