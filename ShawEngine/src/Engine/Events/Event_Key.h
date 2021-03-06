#pragma once
#include "Engine/Events/Event.h"
#include "Engine/Core/KeyCodes.h"

namespace ShawEngine {

	//键盘事件 继承自Event
	class KeyEvent : public Event
	{
	public:
		//获取按键码
		int GetKeyCode() const { return m_KeyCode; }
		//通过宏快速实现事件分类相关
		//GetCategoryFlags()，返回事件类型为：键盘 和 输入
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(const KeyCode keycode)
			: m_KeyCode(keycode) {}
		//按键码
		KeyCode m_KeyCode;
	};

	//按键事件 继承自键盘事件
	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const KeyCode keycode, const uint16_t repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		uint16_t GetRepeatCount() const { return m_RepeatCount; }

		//重写TosTring()，打印事件类型，以及详情
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		//通过宏快速实现事件枚举相关
		EVENT_CLASS_TYPE(KeyPressed)
	private:
		//按键重复计数
		uint16_t m_RepeatCount;
	};

	//释放键事件 继承自键盘事件
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const KeyCode keycode)
			: KeyEvent(keycode) {}

		//重写TosTring()，打印事件类型，以及详情
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		//通过宏快速实现事件枚举相关
		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const KeyCode keycode)
			:KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent:" << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};

}