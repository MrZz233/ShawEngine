#pragma once
#include "Engine/Core.h"
#include <string>
#include <functional>

namespace ShawEngine {

	//目前的事件系统采用阻塞，即当事件发生时，它会立即被调度，而不是推送至事件队列等待下次被调用。

	//定义了事件枚举
	enum class EventType
	{
		None = 0,
		//以下的事件枚举都是一个类，以Event为基类，
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,	//窗口
		AppTick, AppUpdate, AppRender,											//引擎
		KeyPressed, KeyReleased,												//按键
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled		//鼠标
	};

	//事件分类
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),	//0x1		程序本身产生的事件
		EventCategoryInput			= BIT(1),	//0x10		输入事件，包括下面三种
		EventCategoryKeyboard		= BIT(2),	//0x100		键盘
		EventCategoryMouse			= BIT(3),	//0x1000	鼠标
		EventCategoryMouseButton	= BIT(4)	//0x10000	鼠标按键
	};

	//定义了继承Event的子类需要实现的函数的快速实现
	//GetStaticTypeh()返回事件枚举
	//GetEventType()返回事件枚举，子类重写。这样可以通过Event指针返回Event子类的真正类型
	//GetName()返回事件的名称
	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

	//GetCategoryFlags()返回事件分类，子类重写
	#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }
	
	//Event抽象类
	class SE_API Event
	{
		//EventDispatcher事件调度类
		friend class EventDispatcher;

	public:
		//返回事件枚举及类型 接口
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		//返回事件分类 接口
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		//查看事件是否输入当前分类
		inline bool IsInCategory(EventCategory category)
		{
			//与运算，若category在当前分类中，则返回非零
			return GetCategoryFlags() & category;
		}

	protected:
		//事件处理标志
		bool m_Handled = false;
	};

	//EventDispatcher事件调度类
	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;	//EventFn<事件>表示一个函数，参数是事件，返回bool

	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		template<typename T>
		bool Dispatch(EventFn<T> func)	//收到一个事件
		{
			if (m_Event.GetEventType() == T::GetStaticType())	//
			{
				m_Event.m_Handled = func(*(T*)&m_Event);	//执行该事件
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

