#pragma once
//#include "Application.h"
#include "Engine/Core/Base.h"
#include "Engine/Core/Log.h"
#include "Engine/Debug/Instrumentor.h"
#include "Engine/Core/Application.h"

#ifdef SE_PLATFORM_WINDOWS
	//Step1、程序起点
	extern ShawEngine::Application* ShawEngine::CreateApplication(ApplicationCommandLineArgs args);

	int main(int argc, char** argv) {
		ShawEngine::Log::Init();
		SE_PROFILE_BEGIN_SESSION("Startup", "ShawEngineProfile-Startup.json");
		auto app = ShawEngine::CreateApplication({ argc, argv });
		SE_PROFILE_END_SESSION();

		SE_PROFILE_BEGIN_SESSION("Runtime", "ShawEngineProfile-Runtime.json");
		//Step2、程序主循环
		app->Run();		//app指向的是AppStart类型的指针
		SE_PROFILE_END_SESSION();

		SE_PROFILE_BEGIN_SESSION("Shutdown", "ShawEngineProfile-Shutdown.json");
		delete app;
		SE_PROFILE_END_SESSION();
	}

#else
	#error ShawEngine only supports Windows now!
#endif