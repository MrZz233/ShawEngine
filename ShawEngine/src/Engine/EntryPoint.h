#pragma once
//#include "Application.h"

#ifdef SE_PLATFORM_WINDOWS

	int main(int argc, char** argv) {
		ShawEngine::Log::Init();
		SE_CORE_INFO("ShawEngine is intialized!");
		SE_CLIENT_TRACE("Application started!");
		auto app = ShawEngine::CreateApplication();	//Applictaion.h在Engine.h中会被提前引用
		app->Run();		//app指向的是AppStart类型的指针
		delete app;
	}

#else
	#error ShawEngine only supports Windows now!
#endif