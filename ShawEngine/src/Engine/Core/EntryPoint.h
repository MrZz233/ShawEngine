#pragma once
//#include "Application.h"

#ifdef SE_PLATFORM_WINDOWS
	//Step1、程序起点
	int main(int argc, char** argv) {
		ShawEngine::Log::Init();
		SE_CORE_INFO("ShawEngine is intialized!");
		SE_CLIENT_TRACE("Application started!");
		auto app = ShawEngine::CreateApplication();		//AppStart中实现CreateApplication()
														//调用AppStart基类Application的构造函数
														//用Window::Create()返回的值[有默认值]构造WindowsWindow [因为是Windows平台]
														//Init()初始化Application的窗口
		//Step2、程序主循环
		app->Run();		//app指向的是AppStart类型的指针
		delete app;
	}

#else
	#error ShawEngine only supports Windows now!
#endif