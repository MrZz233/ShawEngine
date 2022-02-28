#pragma once
#include "Engine/Core.h"
#include <string>
#include <functional>

namespace ShawEngine {

	//Ŀǰ���¼�ϵͳ���������������¼�����ʱ���������������ȣ��������������¼����еȴ��´α����á�

	//�������¼�ö��
	enum class EventType
	{
		None = 0,
		//���µ��¼�ö�ٶ���һ���࣬��EventΪ���࣬
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,	//����
		AppTick, AppUpdate, AppRender,											//����
		KeyPressed, KeyReleased,												//����
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled		//���
	};

	//�¼�����
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),	//0x1		������������¼�
		EventCategoryInput			= BIT(1),	//0x10		�����¼���������������
		EventCategoryKeyboard		= BIT(2),	//0x100		����
		EventCategoryMouse			= BIT(3),	//0x1000	���
		EventCategoryMouseButton	= BIT(4)	//0x10000	��갴��
	};

	//�����˼̳�Event��������Ҫʵ�ֵĺ����Ŀ���ʵ��
	//GetStaticTypeh()�����¼�ö��
	//GetEventType()�����¼�ö�٣�������д����������ͨ��Eventָ�뷵��Event�������������
	//GetName()�����¼�������
	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

	//GetCategoryFlags()�����¼����࣬������д
	#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }
	
	//Event������
	class SE_API Event
	{
		//EventDispatcher�¼�������
		friend class EventDispatcher;

	public:
		//�����¼�ö�ټ����� �ӿ�
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		//�����¼����� �ӿ�
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		//�鿴�¼��Ƿ����뵱ǰ����
		inline bool IsInCategory(EventCategory category)
		{
			//�����㣬��category�ڵ�ǰ�����У��򷵻ط���
			return GetCategoryFlags() & category;
		}

	protected:
		//�¼������־
		bool m_Handled = false;
	};

	//EventDispatcher�¼�������
	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;	//EventFn<�¼�>��ʾһ���������������¼�������bool

	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		template<typename T>
		bool Dispatch(EventFn<T> func)	//�յ�һ���¼�
		{
			if (m_Event.GetEventType() == T::GetStaticType())	//
			{
				m_Event.m_Handled = func(*(T*)&m_Event);	//ִ�и��¼�
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}

