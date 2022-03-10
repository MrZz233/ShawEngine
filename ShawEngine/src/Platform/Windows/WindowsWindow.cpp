#include "sepch.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Engine/Core/Log.h"
#include "Engine/Events/Event_Application.h"
#include "Engine/Events/Event_Key.h"
#include "Engine/Events/Event_Mouse.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Engine/Renderer/Renderer.h"

namespace ShawEngine {
	
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* desc) {
		SE_CORE_ERROR("GLFW Error:{0} :{1}", error, desc);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		SE_PROFILE_FUNCTION();
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		SE_PROFILE_FUNCTION();
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		SE_PROFILE_FUNCTION();
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		SE_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (s_GLFWWindowCount == 0)
		{
			SE_PROFILE_SCOPE("glfwInit");
			//��ʼ��glfw
			SE_CORE_INFO("Initializing GLFW");
			int success = glfwInit();
			SE_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}
		{
			SE_PROFILE_SCOPE("glfwCreateWindow");
			//����glfw����
#if defined(SE_DEBUG)
			if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}
		
		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		//Set���û�����m_Data��m_Window�����
		glfwSetWindowUserPointer(m_Window, &m_Data);
		//���ô�ֱͬ��
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
		SE_PROFILE_FUNCTION();
		--s_GLFWWindowCount;
		if (s_GLFWWindowCount == 0){
			SE_CORE_INFO("Terminating GLFW");
			glfwTerminate();
		}
	}

	void WindowsWindow::OnUpdate()
	{
		SE_PROFILE_FUNCTION();
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		SE_PROFILE_FUNCTION();
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
