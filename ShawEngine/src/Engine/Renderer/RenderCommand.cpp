#include "sepch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace ShawEngine {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}