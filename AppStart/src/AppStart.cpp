#include <Engine.h>
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp""

class ExampleLayer :public ShawEngine::Layer {
public:
	//用户渲染的层
	//摄像机
	//各个VAO -- VertexArray
	ExampleLayer() :Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f) {

#pragma region 三角形VAO
		//顶点数据
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		//索引数据
		uint32_t indices[3] = { 0, 1, 2 };

		//创建一个VAO
		//VertexArray的成员有 m_RendererID(VAO)、m_VertexBuffers、m_IndexBuffer
		m_VertexArray.reset(ShawEngine::VertexArray::Create());

		//创建一个VBO
		//VertexBuffer的成员有 m_RendererID(VBO)、m_Layout
		std::shared_ptr<ShawEngine::VertexBuffer> vertexBuffer;
		//将原始顶点数据vertices送入VBO
		vertexBuffer.reset(ShawEngine::VertexBuffer::Create(vertices, sizeof(vertices)));
		//设置好布局，方便设置VBO的顶点属性
		ShawEngine::BufferLayout layout = {
			{ ShawEngine::ShaderDataType::Float3, "a_Position" },
			{ ShawEngine::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		
		//VAO绑定顶点属性
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		//创建一个EBO
		//IndexBuffer的成员有 m_RendererID(EBO)、m_Count
		std::shared_ptr<ShawEngine::IndexBuffer> indexBuffer;
		//将原始索引数据indices送入EBO
		indexBuffer.reset(ShawEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		//VAO绑定EBO
		m_VertexArray->SetIndexBuffer(indexBuffer);
#pragma endregion

#pragma region 方形VAO
		//顶点数据
		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};
		//索引数据
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		//创建第二个VAO、VBO、EBO等
		m_SquareVA.reset(ShawEngine::VertexArray::Create());
		std::shared_ptr<ShawEngine::VertexBuffer> squareVB;
		squareVB.reset(ShawEngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShawEngine::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);
		std::shared_ptr<ShawEngine::IndexBuffer> squareIB;
		squareIB.reset(ShawEngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);
#pragma endregion 

#pragma region ShaderSource1
		//Shader
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";
#pragma endregion
		//设置Shader
		m_Shader.reset(new ShawEngine::Shader(vertexSrc, fragmentSrc));

#pragma region ShaderSource2
		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";
#pragma endregion
		//设置Shader
		m_BlueShader.reset(new ShawEngine::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	//更新函数
	void OnUpdate(ShawEngine::Timestep ts) override {
		//控制相机的位置
		if (ShawEngine::Input::IsKeyPressed(SE_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (ShawEngine::Input::IsKeyPressed(SE_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		if (ShawEngine::Input::IsKeyPressed(SE_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (ShawEngine::Input::IsKeyPressed(SE_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		//控制相机的旋转
		if (ShawEngine::Input::IsKeyPressed(SE_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		if (ShawEngine::Input::IsKeyPressed(SE_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		//清屏
		ShawEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		ShawEngine::RenderCommand::Clear();
		//更新相机位置、旋转
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
		//开始渲染场景
		ShawEngine::Renderer::BeginScene(m_Camera);
		//渲染20*20个方形
		//缩放0.1
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				ShawEngine::Renderer::Submit(m_BlueShader, m_SquareVA, transform);
			}
		}
		//渲染三角形
		ShawEngine::Renderer::Submit(m_Shader, m_VertexArray);
		//渲染完毕
		ShawEngine::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		
	}

	void OnEvent(ShawEngine::Event& event) override {

	}

	private:
		//三角形的Shader和VAO
		std::shared_ptr<ShawEngine::Shader> m_Shader;
		std::shared_ptr<ShawEngine::VertexArray> m_VertexArray;

		//方形的Shader和VAO
		std::shared_ptr<ShawEngine::Shader> m_BlueShader;
		std::shared_ptr<ShawEngine::VertexArray> m_SquareVA;

		//摄像机
		ShawEngine::OrthographicCamera m_Camera;
		glm::vec3 m_CameraPosition;
		float m_CameraMoveSpeed = 5.0f;
		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 180.0f;
};


class AppStart :public ShawEngine::Application {
public:
	AppStart() {
		PushLayer(new ExampleLayer());
	}

	~AppStart() {

	}

};

ShawEngine::Application* ShawEngine::CreateApplication() {
	return new AppStart();
}