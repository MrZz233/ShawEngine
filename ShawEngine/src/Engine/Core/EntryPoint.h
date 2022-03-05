#pragma once
//#include "Application.h"

#ifdef SE_PLATFORM_WINDOWS
	//Step1���������
	int main(int argc, char** argv) {
		ShawEngine::Log::Init();
		SE_CORE_INFO("ShawEngine is intialized!");
		SE_CLIENT_TRACE("Application started!");
		auto app = ShawEngine::CreateApplication();		//AppStart��ʵ��CreateApplication()
														//����AppStart����Application�Ĺ��캯��
														//��Window::Create()���ص�ֵ[��Ĭ��ֵ]����WindowsWindow [��Ϊ��Windowsƽ̨]
														//Init()��ʼ��Application�Ĵ���
		//Step2��������ѭ��
		app->Run();		//appָ�����AppStart���͵�ָ��
		delete app;
	}

#else
	#error ShawEngine only supports Windows now!
#endif