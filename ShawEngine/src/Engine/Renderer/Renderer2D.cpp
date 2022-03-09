#include "sepch.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace ShawEngine {

	struct QuadVertex
	{
		//顶点属性 位置、颜色、UV
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		//用来支持Texture
		float TexIndex;	
		float TilingFactor;
	};

	struct Renderer2DData
	{
		//一次DrawCall最大支持平面数量
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;	//一个平面有4个顶点
		static const uint32_t MaxIndices = MaxQuads * 6;	//一个平面有6个索引
		//单个Shader最大支持纹理数
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;	//0表示纯白贴图
		glm::vec4 QuadVertexPositions[4];
		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		SE_PROFILE_FUNCTION();
		//创建VAO（记录VBO和EBO）
		s_Data.QuadVertexArray = VertexArray::Create();
		//创建VBO（存放顶点）
		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		//定义好VBO中的布局
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" }
			});
		//VAO绑定VBO中的顶点属性
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
		//创建顶点基指针
		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
		//创建索引数组
		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			//第一个三角形的索引
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			//第二个三角形的索引
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;
			//两个三角形需要4个顶点，偏移量+4
			offset += 4;
		}
		//创建EBO
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		//索引已经存放到EBO中
		delete[] quadIndices;
		//VAO绑定EBO中的顶点信息
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);		

		//创建原始贴图（1*1大小的白色贴图）
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		//创建采样器数组
		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		//加载Shader
		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		//给片段着色器中的u_Textures编号
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
		
		//第一张贴图默认为原始贴图
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		//4个顶点的位置
		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		SE_PROFILE_FUNCTION();
		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		SE_PROFILE_FUNCTION();
		//绑定Shader
		s_Data.TextureShader->Bind();
		//设置相机
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		//索引计数清零
		s_Data.QuadIndexCount = 0;
		//VBO指针 指向 VBO基址
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		//TextureIndex设置为1
		s_Data.TextureSlotIndex = 1;	//0是纯白贴图
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		SE_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		SE_PROFILE_FUNCTION();
		//计算需要上传的VBO大小  VBO指针 - VBO基址
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		//设置VBO的内容
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
		//准备渲染
		Flush();
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount == 0)
			return;

		//绑定每一张贴图
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);
		//按照索引方式渲染  VAO 索引数量
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		//DrawCalls计数+1
		s_Data.Stats.DrawCalls++;
	}

	//当场景中渲染的平面数量大于设定的最大值，需要分批次渲染
	void Renderer2D::FlushAndReset()
	{
		//由于平面数量已经达到最大值，所以需要把当前的顶点先渲染
		EndScene();
		//渲染完当前的顶点数据后
		//清空Index计数 和 重置VBO指针 ， 重置TextureSlotIndex为1
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	//vec2 position  |  size  |  color
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	//vec3 position  |  size  |  color
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		SE_PROFILE_FUNCTION();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	//实际渲染调用的函数 纯白
	//transform  |  color
	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		SE_PROFILE_FUNCTION();
		//判断当前的平面数量是否已经超过最大值
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			//如果达到限制，就先渲染一批，并重置计数
			FlushAndReset();
		//不提供贴图的话，默认贴图为纯白
		const float textureIndex = 0.0f;
		const float tilingFactor = 1.0f;
		//设置VBO 4个顶点的值
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		//索引计数+6，面数+1
		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}
	
	//vec2 position  |  size  |  texture  | tilingFactor  |  tintColor
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	//vec3 position  |  size  |  texture  | tilingFactor  |  tintColor
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		SE_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}
	
	//实际渲染调用的函数 贴图
	//transform  |  texture  |  tilingColor  |  tintColor
	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor) 
	{
		SE_PROFILE_FUNCTION();
		//渲染并重置计数
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		//constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			//匹配当前纹理和纹理槽中的纹理
			if (*(s_Data.TextureSlots[i]) == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}
		
		//匹配失败，将当前贴图存入贴图槽
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				FlushAndReset();
		}
		//设置VBO 4个顶点的值
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	//vec2 position  |  size  |  rotation  |  color
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	//vec3 position  |  size  |  rotation  |  color
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		SE_PROFILE_FUNCTION();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, color);
	}

	//vec2 position  |  size  |  rotation  |  texture  |  tilingColor  |  tintColor
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	//vec3 position  |  size  |  rotation  |  texture  |  tilingColor  |  tintColor
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		SE_PROFILE_FUNCTION();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}
}