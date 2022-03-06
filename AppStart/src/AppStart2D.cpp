#include "Appstart2D.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

AppStart2D::AppStart2D()
	: Layer("Appstart2D"), m_CameraController(1280.0f / 720.0f)
{
}

void AppStart2D::OnAttach()
{
	SE_PROFILE_FUNCTION();
	m_CheckerboardTexture = ShawEngine::Texture2D::Create("assets/textures/LelouchLogo.png");
	time_last = std::chrono::high_resolution_clock::now();
	time_new = std::chrono::high_resolution_clock::now();
}

void AppStart2D::OnDetach()
{
	SE_PROFILE_FUNCTION();
}

void AppStart2D::OnUpdate(ShawEngine::Timestep ts)
{
	SE_PROFILE_FUNCTION();
	// Update
	m_CameraController.OnUpdate(ts);
	// Render
	{
		SE_PROFILE_SCOPE("Renderer Prep");
		ShawEngine::RenderCommand::SetClearColor({ 0.2f, 0.4f, 0.4f, 1 });
		ShawEngine::RenderCommand::Clear();
	}
	// Draw
	{
		SE_PROFILE_SCOPE("Renderer Draw");
		ShawEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		ShawEngine::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, m_SquareColor1);
		ShawEngine::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor2);
		
		//开启了深度测试，背景的深度设置为-0.1
		//ShawEngine::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, 0.0f, m_CheckerboardTexture,10.0f);
		//ShawEngine::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, 10.0f);

		ShawEngine::Renderer2D::EndScene();
	}
	++fps_count;
	time_new = std::chrono::high_resolution_clock::now();
	long long start = std::chrono::time_point_cast<std::chrono::microseconds>(time_last).time_since_epoch().count();
	long long end = std::chrono::time_point_cast<std::chrono::microseconds>(time_new).time_since_epoch().count();
	float duration = (end - start) * 0.001f;
	if (duration > 999) {
		fps = fps_count;
		fps_count = 0;
		time_last = time_new;
	}
}

void AppStart2D::OnImGuiRender()
{
	SE_PROFILE_FUNCTION();
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color_1", glm::value_ptr(m_SquareColor1));
	ImGui::ColorEdit4("Square Color_2", glm::value_ptr(m_SquareColor2));
	ImGui::Text("FPS: %d", fps);

	ImGui::End();
}

void AppStart2D::OnEvent(ShawEngine::Event& e)
{
	m_CameraController.OnEvent(e);
}