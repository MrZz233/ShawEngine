#include "sepch.h"
#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ShawEngine {

	class CameraController : public ScriptableEntity
	{
	public:
		virtual void OnCreate() override
		{
			auto& translation = GetComponent<TransformComponent>().Translation;
			//transform[3][0] = rand() % 10 - 5.0f;
			translation.z = 5.0f;
		}

		virtual void OnDestroy() override
		{
		}

		virtual void OnUpdate(Timestep ts) override
		{
			auto& translation = GetComponent<TransformComponent>().Translation;
			float speed = 2.0f;
			float rot_speed = 0.1f;
			if (EditorLayer::GetViewportFocus()) {
				if (Input::IsKeyPressed(Key::A))
					translation.x -= speed * ts;
				if (Input::IsKeyPressed(Key::D))
					translation.x += speed * ts;
				if (Input::IsKeyPressed(Key::W))
					translation.y += speed * ts;
				if (Input::IsKeyPressed(Key::S))
					translation.y -= speed * ts;
				if (Input::IsMouseButtonPressed(Mouse::Button0)) {
					auto& rotation = GetComponent<TransformComponent>().Rotation;
					glm::vec2 mouse_pos_new = Input::GetMousePosition();
					rotation.y -= rot_speed * glm::radians((mouse_pos_new - mouse_pos_last).x);
					rotation.x -= rot_speed * glm::radians((mouse_pos_new - mouse_pos_last).y);
				}
			}
			mouse_pos_last = Input::GetMousePosition();
			//std::cout << "mouse_pos: " << mouse_pos_last.x << mouse_pos_last.y << "\n";
		}
	private:
		glm::vec2 mouse_pos_last{};
	};

	bool EditorLayer::m_ViewportFocused = false;
	bool EditorLayer::m_ViewportHovered = false;

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
	}

	void EditorLayer::OnAttach()
	{
		SE_PROFILE_FUNCTION();

		m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_LelouchTexture = Texture2D::Create("assets/textures/LelouchLogo.png");
		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		//创建一个场景
		m_ActiveScene = CreateRef<Scene>();
		//创建一个square实体
		m_SquareEntity = m_ActiveScene->CreateEntity("Green Square");
		//给square添加SpriteRendererComponent
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f }, Shape::Quad);
		m_SquareEntity.GetComponent<TransformComponent>().Translation = { -0.3f,0.2f,-2.0f };

		auto redSquare = m_ActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f }, Shape::Quad);
		redSquare.GetComponent<TransformComponent>().Translation = { 0.2f,-0.3f,2.0f };

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraEntity.AddComponent<CameraComponent>();
		
		m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>(); 
		
		cc.Primary = false;

		//Bind<CameraController>() 指定了该nsc组件中的Create、Destroy、Update等
		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{
		SE_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		static auto time_last = std::chrono::high_resolution_clock::now();
		static auto time_new = std::chrono::high_resolution_clock::now();
		static uint32_t fps_count = 0;
		SE_PROFILE_FUNCTION();
		//默认创建的FrameBuffer大小为1280*720
		FramebufferSpecification spec = m_Framebuffer->GetSpecification();
		if ( m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			std::cout << "Size Change" << "\n";
		}

		// Update
		//if (m_ViewportFocused)
			//m_CameraController.OnUpdate(ts);

		// Render初始化
		Renderer2D::ResetStats();
		//清空背景
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();
		//Update场景
		//Renderer2D::BeginScene(m_CameraController.GetCamera());
		m_ActiveScene->OnUpdate(ts);
		//Renderer2D::EndScene();
		m_Framebuffer->Unbind();

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

					if (ImGui::MenuItem("Exit")) Application::Get().Close();
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			m_SceneHierarchyPanel.OnImGuiRender();

			ImGui::Begin("Information:");
			auto stats = Renderer2D::GetStats();
			ImGui::Text("FPS: %d", fps);
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
			
			//切换主摄像机
			if (ImGui::Checkbox("Camera A", &m_PrimaryCamera))
			{
				m_CameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
				m_SecondCamera.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
			}
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
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			uint64_t TextureID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image(reinterpret_cast<void*>(TextureID), ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::End();

			ImGui::Begin("DepthSpace");
			ImVec2 viewportPanelSize2 = ImGui::GetContentRegionAvail();
			uint64_t depthTextureID = m_Framebuffer->GetDepthAttachmentRendererID();
			ImGui::Image(reinterpret_cast<void*>(depthTextureID), ImVec2{ viewportPanelSize2.x, viewportPanelSize2.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::End();

			ImGui::End();
		}
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}

}