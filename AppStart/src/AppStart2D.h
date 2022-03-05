#pragma once

#include "Engine.h"

class AppStart2D : public ShawEngine::Layer
{
public:
	AppStart2D();
	virtual ~AppStart2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(ShawEngine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(ShawEngine::Event& e) override;
private:
	ShawEngine::OrthographicCameraController m_CameraController;

	// Temp
	ShawEngine::Ref<ShawEngine::VertexArray> m_SquareVA;
	ShawEngine::Ref<ShawEngine::Shader> m_FlatColorShader;
	ShawEngine::Ref<ShawEngine::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_SquareColor1 = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec4 m_SquareColor2 = { 0.8f, 0.3f, 0.2f, 1.0f };

};