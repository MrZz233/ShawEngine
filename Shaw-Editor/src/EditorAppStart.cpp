#include <Engine.h>
#include <Engine/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace ShawEngine {

	class ShawEngineEditor : public Application
	{
	public:
		ShawEngineEditor(ApplicationCommandLineArgs args)
			: Application("ShawEngine Editor", args)
		{
			PushLayer(new EditorLayer());
		}

		~ShawEngineEditor()
		{
		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new ShawEngineEditor(args);
	}

}