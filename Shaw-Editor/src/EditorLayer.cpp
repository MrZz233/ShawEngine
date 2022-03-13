#include "sepch.h"
#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Utils/PlatformUtils.h"

#include "ImGuizmo.h"

#include "Engine/Math/Math.h"

namespace ShawEngine {

	class CameraController : public ScriptableEntity
	{
	public:
		virtual void OnCreate() override
		{
			auto& transform = GetComponent<TransformComponent>();
			//transform[3][0] = rand() % 10 - 5.0f;
			transform.Translation.z = 5.0f;
			transform.Rotation.y = 0.0f;
		}

		virtual void OnDestroy() override
		{
		}

		virtual void OnUpdate(Timestep ts) override
		{
			if (!GetComponent<CameraComponent>().Primary)
				return;
			auto& transform = GetComponent<TransformComponent>();
			auto& translation = transform.Translation;
			float speed = 2.0f;
			float rot_speed = 0.1f;
			if (EditorLayer::GetViewportFocus()) {
				auto& rotation = transform.Rotation;
				if (Input::IsMouseButtonPressed(Mouse::Button0)) {
					glm::vec2 mouse_pos_new = Input::GetMousePosition();
					//相当于偏航角yaw
					rotation.y -= rot_speed * (mouse_pos_new - mouse_pos_last).x;
					//相当于仰俯角pitch
					rotation.x -= rot_speed * (mouse_pos_new - mouse_pos_last).y;
					if (rotation.x > 89.0f)
						rotation.x = 89.0f;
					if (rotation.x < -89.0f)
						rotation.x = -89.0f;
				}
				float yaw = glm::radians(rotation.y);
				if (Input::IsKeyPressed(Key::Left)) {
					translation.x -= speed * ts * glm::cos(yaw);
					translation.z += speed * ts * glm::sin(yaw);
				}
				if (Input::IsKeyPressed(Key::Right)) {
					translation.x += speed * ts * glm::cos(yaw);
					translation.z -= speed * ts * glm::sin(yaw);
				}
				if (Input::IsKeyPressed(Key::Up)) {
					translation.x -= speed * ts * glm::sin(yaw);
					translation.z -= speed * ts * glm::cos(yaw);
				}
				if (Input::IsKeyPressed(Key::Down)) {
					translation.x += speed * ts * glm::sin(yaw);
					translation.z += speed * ts * glm::cos(yaw);
				}
				if (Input::IsKeyPressed(Key::Space))
					translation.y += speed * ts;
				if (Input::IsKeyPressed(Key::RightControl))
					translation.y -= speed * ts;
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
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth }; 
		m_Framebuffer = Framebuffer::Create(fbSpec);

		//创建一个场景
		m_ActiveScene = CreateRef<Scene>();

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 1.0f, 1000.0f);
#if 0
		//创建一个square实体
		m_SquareEntity = m_ActiveScene->CreateEntity("Green Square");
		//给square添加SpriteRendererComponent
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f }, Shape::Quad);
		m_SquareEntity.GetComponent<TransformComponent>().Translation = { -0.3f,0.0f,-2.0f };

		auto redSquare = m_ActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f }, Shape::Quad);
		redSquare.GetComponent<TransformComponent>().Translation = { 0.2f,0.0f,2.0f };

		auto _Plane = m_ActiveScene->CreateEntity("Plane");
		_Plane.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.8f, 0.8f, 0.8f, 1.0f }, Shape::Quad);
		_Plane.GetComponent<TransformComponent>().Translation = { 0.0f,-0.5f,0.0f };
		_Plane.GetComponent<TransformComponent>().Rotation = { 90.0f,0.0f,0.0f };
		_Plane.GetComponent<TransformComponent>().Scale = { 10.0f,10.0f,10.0f };

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraEntity.AddComponent<CameraComponent>();
		
		m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>(); 
		
		cc.Primary = false;

		//Bind<CameraController>() 指定了该nsc组件中的Create、Destroy、Update等
		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif



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
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			std::cout << "Size Change" << "\n";
		}

		// Update
		//if (m_ViewportFocused)
			//m_CameraController.OnUpdate(ts);
		m_EditorCamera.OnUpdate(ts);

		// Render初始化
		Renderer2D::ResetStats();
		//清空背景
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();
		m_Framebuffer->ClearAttachment(1, -1);
		//Update场景
		//Renderer2D::BeginScene(m_CameraController.GetCamera());
		m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
		auto [mx, my] = ImGui::GetMousePos();	//鼠标在整个屏幕上的位置
		
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			SE_CORE_WARN("Pixel data = {0}", pixelData);
		}
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
			ImGuiStyle& style = ImGui::GetStyle();
			float minWinSizeX = style.WindowMinSize.x;
			style.WindowMinSize.x = 256.0f;
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}
			style.WindowMinSize.x = minWinSizeX;
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Option"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows, 
					// which we can't undo at the moment without finer window depth/z control.
					//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
					if (ImGui::MenuItem("New", "Ctrl+N"))
						NewScene();

					if (ImGui::MenuItem("Open...", "Ctrl+O"))
						OpenScene();

					if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
						SaveSceneAs();

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
			bool _block = !ImGui::IsWindowFocused() && !ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->BlockEvents(_block);
			
			auto viewportOffset = ImGui::GetCursorPos(); // Includes tab bar
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			uint64_t TextureID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image(reinterpret_cast<void*>(TextureID), ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			//auto windowSize = ImGui::GetWindowSize();
			ImVec2 minBound = ImGui::GetWindowPos();	//GetWindowPos返回Viewport左上角在整个屏幕的位置
			minBound.x += viewportOffset.x;
			minBound.y += viewportOffset.y;	//viewport除去bar之后，左上角在在整个屏幕的位置
			std::cout << minBound.x << "  " << minBound.y << "\n";
			ImVec2 maxBound = { minBound.x + viewportPanelSize.x, minBound.y + viewportPanelSize.y };
			m_ViewportBounds[0] = { minBound.x, minBound.y };
			m_ViewportBounds[1] = { maxBound.x, maxBound.y };
			
			//Gizmos
			Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
			if (selectedEntity && m_GizmoType != -1)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				float windowWidth = (float)ImGui::GetWindowWidth();
				float windowHeight = (float)ImGui::GetWindowHeight();
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

				// Camera
				//auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
				//const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
				//const glm::mat4& cameraProjection = camera.GetProjection();
				//glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

				const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
				glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();
				// Entity transform
				auto& tc = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = tc.GetTransform();

				// Snapping
				bool snap = Input::IsKeyPressed(Key::LeftControl);
				float snapValue = 0.5f; // Snap to 0.5m for translation/scale
				// Snap to 45 degrees for rotation
				if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;

				float snapValues[3] = { snapValue, snapValue, snapValue };

				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
					nullptr, snap ? snapValues : nullptr);

				if (ImGuizmo::IsUsing())
				{
					glm::vec3 translation, rotation, scale;
					Math::DecomposeTransform(transform, translation, rotation, scale);
					glm::vec3 deltaRotation = rotation - tc.Rotation;
					tc.Translation = translation;
					tc.Rotation += deltaRotation;
					tc.Scale = scale;
				}
			}
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
		//m_CameraController.OnEvent(e);
		m_EditorCamera.OnEvent(e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(SE_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (e.GetKeyCode())
		{
			case Key::N:
			{
				if (control)
					NewScene();

				break;
			}
			case Key::O:
			{
				if (control)
					OpenScene();

				break;
			}
			case Key::S:
			{
				if (control && shift)
					SaveSceneAs();

				break;
			}
			// Gizmos
			case Key::Q:
				m_GizmoType = -1;
				break;
			case Key::W:
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case Key::E:
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case Key::R:
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
		}
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::optional<std::string> filepath = FileDialogs::OpenFile("Scene (*.sc)\0*.sc\0");
		if (filepath)
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(*filepath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::optional<std::string> filepath = FileDialogs::SaveFile("Scene (*.sc)\0*.sc\0");
		if (filepath)
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(*filepath);
		}
	}

}