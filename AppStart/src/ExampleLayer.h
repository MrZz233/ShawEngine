#pragma once

#include "Engine.h"

class ExampleLayer : public ShawEngine::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(ShawEngine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(ShawEngine::Event& e) override;
private:
	ShawEngine::ShaderLibrary m_ShaderLibrary;
	ShawEngine::Ref<ShawEngine::Shader> m_Shader;
	ShawEngine::Ref<ShawEngine::VertexArray> m_VertexArray;

	ShawEngine::Ref<ShawEngine::Shader> m_FlatColorShader;
	ShawEngine::Ref<ShawEngine::VertexArray> m_SquareVA;

	ShawEngine::Ref<ShawEngine::Texture2D> m_Texture, m_ChernoLogoTexture;

	ShawEngine::OrthographicCameraController m_CameraController;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};