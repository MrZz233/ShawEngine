#pragma once

#include "Engine.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Engine/Renderer/EditorCamera.h"

namespace ShawEngine {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
		static bool GetViewportFocus() { return m_ViewportFocused; };
	
	private:
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();

		ShawEngine::OrthographicCameraController m_CameraController;

		// Temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;

		Entity m_CameraEntity;
		Entity m_SecondCamera;

		bool m_PrimaryCamera = true;
		EditorCamera m_EditorCamera;
		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Texture2D> m_LelouchTexture;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		uint32_t fps = 0;

		static bool m_ViewportFocused, m_ViewportHovered;
		int m_GizmoType = -1;
		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}
