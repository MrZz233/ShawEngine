#pragma once
#include "Engine/Events/Event.h"

namespace ShawEngine {

	//�����¼� �̳���Event
	class KeyEvent : public Event
	{
	public:
		//��ȡ������
		inline int GetKeyCode() const { return m_KeyCode; }
		//ͨ�������ʵ���¼��������
		//GetCategoryFlags()�������¼�����Ϊ������ �� ����
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}
		//������
		int m_KeyCode;
	};

	//�����¼� �̳��Լ����¼�
	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		//��дTosTring()����ӡ�¼����ͣ��Լ�����
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		//ͨ�������ʵ���¼�ö�����
		EVENT_CLASS_TYPE(KeyPressed)
	private:
		//�����ظ�����
		int m_RepeatCount;
	};

	//�ͷż��¼� �̳��Լ����¼�
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		//��дTosTring()����ӡ�¼����ͣ��Լ�����
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		//ͨ�������ʵ���¼�ö�����
		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
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