#pragma once
#include "Event.h"
#include <sstream>

//Window��App��ص���
namespace ShawEngine {
	
	//�������ڴ�С�¼����̳���Event
	class SE_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }
		
		//��дTosTring()����ӡ�¼����ͣ��Լ�����
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		//ͨ�������ʵ���¼�ö�ٺͷ������
		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};

	//���ڹر��¼����̳���Event
	class SE_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		//ͨ�������ʵ���¼�ö�ٺͷ������
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	//�������¼����̳���Event
	class SE_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		//ͨ�������ʵ���¼�ö�ٺͷ������
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	//������ѭ���¼����̳���Event
	class SE_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		//ͨ�������ʵ���¼�ö�ٺͷ������
		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	//������Ⱦ�¼����̳���Event
	class SE_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		//ͨ�������ʵ���¼�ö�ٺͷ������
		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}