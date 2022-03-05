#include <Engine.h>
#include <Engine/Core/EntryPoint.h>
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"

#include "AppStart2D.h"

class ExampleLayer :public ShawEngine::Layer {
public:
	//用户渲染的层
	//摄像机
	//各个VAO -- VertexArray
	ExampleLayer() :Layer("Example"), m_CameraController(1280.0f / 720.0f) {

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
		m_VertexArray = ShawEngine::VertexArray::Create();

		//创建一个VBO
		//VertexBuffer的成员有 m_RendererID(VBO)、m_Layout
		ShawEngine::Ref<ShawEngine::VertexBuffer> vertexBuffer;
		//将原始顶点数据vertices送入VBO
		vertexBuffer = ShawEngine::VertexBuffer::Create(vertices, sizeof(vertices));
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
		ShawEngine::Ref<ShawEngine::IndexBuffer> indexBuffer;
		//将原始索引数据indices送入EBO
		indexBuffer = ShawEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		//VAO绑定EBO
		m_VertexArray->SetIndexBuffer(indexBuffer);
#pragma endregion

#pragma region 方形VAO
		//顶点数据
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		//索引数据
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		//创建第二个VAO、VBO、EBO等
		m_SquareVA = ShawEngine::VertexArray::Create();
		ShawEngine::Ref<ShawEngine::VertexBuffer> squareVB;
		squareVB = ShawEngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ ShawEngine::ShaderDataType::Float3, "a_Position" },
			{ ShawEngine::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);
		ShawEngine::Ref<ShawEngine::IndexBuffer> squareIB;
		squareIB = ShawEngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
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
		m_Shader = ShawEngine::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

#pragma region ShaderSource2
		std::string flatColorShaderVertexSrc = R"(
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

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			uniform vec3 u_Color;
			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";
#pragma endregion
		//设置Shader
		m_FlatColorShader = ShawEngine::Shader::Create("FlatColor",flatColorShaderVertexSrc, flatColorShaderFragmentSrc);
	
		//从文件读取shader
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = ShawEngine::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = ShawEngine::Texture2D::Create("assets/textures/LelouchLogo.png");

		std::dynamic_pointer_cast<ShawEngine::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<ShawEngine::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);

	}

	//更新函数
	void OnUpdate(ShawEngine::Timestep ts) override {
		m_CameraController.OnUpdate(ts);
		
		//清屏
		ShawEngine::RenderCommand::SetClearColor({ 0.2f, 0.4f, 0.4f, 1 });
		ShawEngine::RenderCommand::Clear();
		
		//开始渲染场景
		ShawEngine::Renderer::BeginScene(m_CameraController.GetCamera());
		//渲染20*20个方形
		//缩放0.1
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<ShawEngine::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<ShawEngine::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				ShawEngine::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
				
			}
		}
		//渲染三角形
		//ShawEngine::Renderer::Submit(m_Shader, m_VertexArray);
		auto textureShader = m_ShaderLibrary.Get("Texture");
		//背景
		m_Texture->Bind();
		ShawEngine::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		//logo
		m_ChernoLogoTexture->Bind();
		ShawEngine::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		//渲染完毕
		ShawEngine::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(ShawEngine::Event& e) override {
		m_CameraController.OnEvent(e);
	}

	private:

		ShawEngine::ShaderLibrary m_ShaderLibrary;

		//三角形的Shader和VAO
		ShawEngine::Ref<ShawEngine::Shader> m_Shader;
		ShawEngine::Ref<ShawEngine::VertexArray> m_VertexArray;

		//方形的Shader和VAO
		ShawEngine::Ref<ShawEngine::Shader> m_FlatColorShader;
		ShawEngine::Ref<ShawEngine::VertexArray> m_SquareVA;

		//贴图
		ShawEngine::Ref<ShawEngine::Texture2D> m_Texture, m_ChernoLogoTexture;
		
		//摄像机
		ShawEngine::OrthographicCameraController m_CameraController;

		glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};


class AppStart :public ShawEngine::Application {
public:
	AppStart() {
		//PushLayer(new ExampleLayer());
		PushLayer(new AppStart2D());
	}

	~AppStart() {

	}

};

ShawEngine::Application* ShawEngine::CreateApplication() {
	return new AppStart();
}