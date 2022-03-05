#include "Appstart2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

AppStart2D::AppStart2D()
	: Layer("Appstart2D"), m_CameraController(1280.0f / 720.0f)
{
}

void AppStart2D::OnAttach()
{
	m_CheckerboardTexture = ShawEngine::Texture2D::Create("assets/textures/LelouchLogo.png");
}

void AppStart2D::OnDetach()
{
}

void AppStart2D::OnUpdate(ShawEngine::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	ShawEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	ShawEngine::RenderCommand::Clear();

	ShawEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	
	//std::dynamic_pointer_cast<ShawEngine::OpenGLShader>(m_FlatColorShader)->Bind();
	//std::dynamic_pointer_cast<ShawEngine::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	ShawEngine::Renderer2D::DrawQuad({ -0.85f* (1280.0f / 720.0f), 0.85f }, { 0.2f, 0.2f }, m_SquareColor1);
	ShawEngine::Renderer2D::DrawQuad({ -0.65f* (1280.0f / 720.0f), 0.85f }, { 0.2f, 0.2f }, m_SquareColor2);
	//开启了深度测试，背景的深度设置为-1
	ShawEngine::Renderer2D::DrawQuad({ 0.0f, 0.0f, -1.0f }, { 10.0f, 10.0f }, m_CheckerboardTexture);
	
	ShawEngine::Renderer2D::EndScene();
}

void AppStart2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color_1", glm::value_ptr(m_SquareColor1));
	ImGui::ColorEdit4("Square Color_2", glm::value_ptr(m_SquareColor2));

	ImGui::End();
}

void AppStart2D::OnEvent(ShawEngine::Event& e)
{
	m_CameraController.OnEvent(e);
}