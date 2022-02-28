#pragma once
#include "Event.h"
#include <sstream>

//Window和App相关的类
namespace ShawEngine {
	
	//调整窗口大小事件，继承自Event
	class SE_API WindowResizeEvent : public Event
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
	class SE_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		//通过宏快速实现事件枚举和分类相关
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	//程序点击事件，继承自Event
	class SE_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		//通过宏快速实现事件枚举和分类相关
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	//程序主循环事件，继承自Event
	class SE_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		//通过宏快速实现事件枚举和分类相关
		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	//程序渲染事件，继承自Event
	class SE_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		//通过宏快速实现事件枚举和分类相关
		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}