#include "sepch.h"
#include "WindowsWindow.h"
#include "Engine/Log.h"
#include "Engine/Events/Event_Application.h"
#include "Engine/Events/Event_Key.h"
#include "Engine/Events/Event_Mouse.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace ShawEngine {
	
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* desc) {
		SE_CORE_ERROR("GLFW Error:{0} :{1}", error, desc);
	}

	//Ĭ��propsΪWindowProps()���洢��title��width��height
	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		SE_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			//��ʼ��glfw
			int success = glfwInit();
			SE_CORE_ASSERT(success, "Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		//����glfw����
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		//Set���û�����m_Data��m_Window�����
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
		//����glfw�ص�
		//�������ڴ�С
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			//Get��ȡ�뵱ǰwindow��������û�����ָ��
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent  e(width, height);
			data.EventCallback(e);	
		});
		//�رմ���
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent  e;
			data.EventCallback(e);
		});
		//����
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window,int key,int scancode,int action,int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action) {
				case GLFW_PRESS:
				{
					KeyPressedEvent e(key, 0);
					data.EventCallback(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent e(key, 1);
					data.EventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent e(key);
					data.EventCallback(e);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent e(keycode);
			data.EventCallback(e);
		});

		//��갴��
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button,int action, int mods) {
			//Get��ȡ�뵱ǰwindow��������û�����ָ��
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action) {
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent e(button);
					data.EventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent e(button);
					data.EventCallback(e);
					break;
				}
			}
		});
		//������
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double x, double y) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent e((float)x, (float)y);
			data.EventCallback(e);
		});
		//����ƶ�
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent e((float)x, (float)y);
			data.EventCallback(e);
		});

	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}
