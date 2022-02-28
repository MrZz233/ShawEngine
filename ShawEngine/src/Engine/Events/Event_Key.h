#pragma once
#include "Event.h"
#include <sstream>

namespace ShawEngine {

	//�����¼� �̳���Event
	class SE_API KeyEvent : public Event
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
	class SE_API KeyPressedEvent : public KeyEvent
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
	class SE_API KeyReleasedEvent : public KeyEvent
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
}