#pragma once
//#include "Application.h"
#include "Engine/Core/Core.h"
#include "Engine/Core/Log.h"

#ifdef SE_PLATFORM_WINDOWS
	//Step1���������
	int main(int argc, char** argv) {
		ShawEngine::Log::Init();
		SE_PROFILE_BEGIN_SESSION("Startup", "ShawEngineProfile-Startup.json");
		auto app = ShawEngine::CreateApplication();	
		SE_PROFILE_END_SESSION();

		SE_PROFILE_BEGIN_SESSION("Runtime", "ShawEngineProfile-Runtime.json");
		//Step2��������ѭ��
		app->Run();		//appָ�����AppStart���͵�ָ��
		SE_PROFILE_END_SESSION();

		SE_PROFILE_BEGIN_SESSION("Shutdown", "ShawEngineProfile-Shutdown.json");
		delete app;
		SE_PROFILE_END_SESSION();
	}

#else
	#error ShawEngine only supports Windows now!
#endif