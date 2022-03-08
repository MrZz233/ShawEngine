#pragma once
//供AppStart使用
#include "Engine/Core/Application.h"
//提供日志系统
#include "Engine/Core/Base.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/Layer.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/MouseCodes.h"
#include "Engine/Renderer/OrthographicCameraController.h"
//ImGui层
#include "Engine/ImGui/ImGuiLayer.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Components.h"

// ---Renderer------------------------
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Framebuffer.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/VertexArray.h"

#include "Engine/Renderer/OrthographicCamera.h"
// -----------------------------------