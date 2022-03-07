#include "EditorLayer.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ShawEngine {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
	}

	void EditorLayer::OnAttach()
	{
		SE_PROFILE_FUNCTION();

		m_CheckerboardTexture = ShawEngine::Texture2D::Create("assets/textures/Checkerboard.png");
		m_LelouchTexture = ShawEngine::Texture2D::Create("assets/textures/LelouchLogo.png");
		ShawEngine::FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = ShawEngine::Framebuffer::Create(fbSpec);
	}

	void EditorLayer::OnDetach()
	{
		SE_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(ShawEngine::Timestep ts)
	{
		static auto time_last = std::chrono::high_resolution_clock::now();
		static auto time_new = std::chrono::high_resolution_clock::now();
		static uint32_t fps_count = 0;
		SE_PROFILE_FUNCTION();
		// Update
		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		// Render
		ShawEngine::Renderer2D::ResetStats();
		{
			SE_PROFILE_SCOPE("Renderer Prep");
			m_Framebuffer->Bind();
			ShawEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			ShawEngine::RenderCommand::Clear();
		}

		{
			static float rotation = 0.0f;
			rotation += ts * 50.0f;

			SE_PROFILE_SCOPE("Renderer Draw");
			ShawEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
			ShawEngine::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.6f }, rotation, m_SquareColor);
			ShawEngine::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, m_SquareColor);
			ShawEngine::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 3.0f, 3.0f }, m_CheckerboardTexture, 5.0f);
			ShawEngine::Renderer2D::DrawQuad({ -0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_LelouchTexture, 3.0f);
			ShawEngine::Renderer2D::EndScene();

			ShawEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
			for (float y = -1.0f; y < 1.0f; y += 0.2f)
			{
				for (float x = -1.0f; x < 1.0f; x += 0.2f)
				{
					glm::vec4 color = { (x + 1.0f) / 2.0f, 0.4f, (y + 1.0f) / 2.0f, 0.5f };
					ShawEngine::Renderer2D::DrawRotatedQuad({ x , y }, { 0.1f, 0.1f }, rotation, color);
				}
			}
			ShawEngine::Renderer2D::EndScene();
			m_Framebuffer->Unbind();
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

	void EditorLayer::OnImGuiRender()
	{
		SE_PROFILE_FUNCTION();

		//是否启动停靠
		static bool dockingEnabled = true;
		if (dockingEnabled)
		{
			static bool dockspaceOpen = true;
			static bool opt_fullscreen_persistant = true;
			bool opt_fullscreen = opt_fullscreen_persistant;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			//ImGuiWindowFlags_NoDocking 防止父窗口停靠
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			//ImGuiDockNodeFlags_PassthruCentralNode 绘制DockSpace的背景
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
			// all active windows docked into it will lose their parent and become undocked.
			// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
			// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
			ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// DockSpace
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Option"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows, 
					// which we can't undo at the moment without finer window depth/z control.
					//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

					if (ImGui::MenuItem("Exit")) ShawEngine::Application::Get().Close();
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			ImGui::Begin("Settings");
			auto stats = ShawEngine::Renderer2D::GetStats();
			ImGui::Text("FPS: %d", fps);
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
			ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
			ImGui::End();

			ImGui::Begin("Viewport");
			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			//BlockEvents为true   ImGui处理事件，
			//没有聚焦  或者  没有悬浮  ImGui才处理事件
			//等价于   聚焦且悬浮   ImGui不处理事件  
			bool _block = !m_ViewportFocused || !m_ViewportHovered;
			Application::Get().GetImGuiLayer()->BlockEvents(_block);

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			if (m_ViewportSize != *((glm::vec2*) & viewportPanelSize))
			{
				//调整渲染图像的大小
				m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
				//根据ImGui窗口大小调整ViewPort
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
				//根据ViewPort调整相机的视角
				m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
			}
			uint32_t TextureID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)TextureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::End();

			ImGui::Begin("DepthSpace");
			ImVec2 viewportPanelSize2 = ImGui::GetContentRegionAvail();
			uint32_t depthTextureID = m_Framebuffer->GetDepthAttachmentRendererID();
			ImGui::Image((void*)depthTextureID, ImVec2{ viewportPanelSize2.x, viewportPanelSize2.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::End();

			ImGui::End();
		}
	}

	void EditorLayer::OnEvent(ShawEngine::Event& e)
	{
		m_CameraController.OnEvent(e);
	}

}