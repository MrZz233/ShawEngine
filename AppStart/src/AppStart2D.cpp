#include "Appstart2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

AppStart2D::AppStart2D()
	: Layer("Appstart2D"), m_CameraController(1280.0f / 720.0f)
{
}

void AppStart2D::OnAttach()
{

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

	ShawEngine::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_SquareColor);
	ShawEngine::Renderer2D::EndScene();

	// TODO: Add these functions - Shader::SetMat4, Shader::SetFloat4

}

void AppStart2D::OnImGuiRender()
{
	ImGui::Begin("Settings_1");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void AppStart2D::OnEvent(ShawEngine::Event& e)
{
	m_CameraController.OnEvent(e);
}