#pragma once
//#include "Application.h"

#ifdef SE_PLATFORM_WINDOWS
	
	int main(int argc, char** argv) {
		auto app = ShawEngine::CreateApplication();	//Applictaion.h��Engine.h�лᱻ��ǰ����
		app->Run();
		delete app;
	}

#else
	#error ShawEngine only supports Windows now!
#endif