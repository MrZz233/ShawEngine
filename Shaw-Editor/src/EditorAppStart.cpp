#include <Engine.h>
#include <Engine/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace ShawEngine {

	class ShawEngineEditor : public Application
	{
	public:
		ShawEngineEditor()
			: Application("ShawEngine Editor")
		{
			PushLayer(new EditorLayer());
		}

		~ShawEngineEditor()
		{
		}
	};

	Application* CreateApplication()
	{
		return new ShawEngineEditor();
	}

}