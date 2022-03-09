#include "sepch.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Components.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace ShawEngine {

	static void DoMath(const glm::mat4& transform)
	{
	}

	static void OnTransformConstruct(entt::registry& registry, entt::entity entity)
	{
	}

	Scene::Scene()
	{
#if ENTT_EXAMPLE_CODE
		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();

		if (m_Registry.has<TransformComponent>(entity))
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);

		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view){
			TransformComponent& transform = view.get<TransformComponent>(entity);
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group){
			auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
		}
#endif
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		// Update scripts
		{
			//找出有NativeScript组件的entt::entity
			//匿名函数 entity表示实体ID，nsc表示实际挂载的NativeScript组件
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					//nsc.Instance指向一个ScriptableEntity，比如CameraController
					//如果nsc中的Instance还没有指向任何脚本物体，就执行初始化函数
					if (!nsc.Instance)
					{
						nsc.InstantiateFunction();	//让nsc.Instance指向实际的类型new T()，T继承自ScriptableEntity
						//给Instance中m_Enitity赋值{entt::entity,场景指针}
						nsc.Instance->m_Entity = Entity{ entity, this };
						//如果nsc中的OnCreate不为空，就执行
						if (nsc.OnCreateFunction)
							//因为ScriptableEntity中没有OnCreate，所以不能按下面方式调用
							//nsc.Instance->OnCreate()
							nsc.OnCreateFunction();
					}
					//如果nsc中的OnUpdate不为空，就执行
					if (nsc.OnUpdateFunction)
						nsc.OnUpdateFunction(ts);
				});
		}

		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), * cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform, sprite.Color);
			}

			Renderer2D::EndScene();
		}

	}

	/*
	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}*/

	
	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}

	}

}