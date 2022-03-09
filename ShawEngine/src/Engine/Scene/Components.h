#pragma once
#include "Engine/Scene/SceneCamera.h"
#include "Engine/Scene/ScriptableEntity.h"
#include <glm/glm.hpp>

namespace ShawEngine {

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true; // TODO: think about moving to Scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

	};

	struct NativeScriptComponent
	{
		//nsc存放了脚本物体的指针
		ScriptableEntity* Instance = nullptr;

		//nsc用于创建和销毁Instance的函数
		std::function<void()> InstantiateFunction;
		std::function<void()> DestroyInstanceFunction;

		//通过nsc调用脚本物体中的OnCreate、OnDestroy、OnUpdate
		std::function<void()> OnCreateFunction;
		std::function<void()> OnDestroyFunction;
		std::function<void(Timestep)> OnUpdateFunction;

		//添加脚本组件的时候，通过泛型实现执行不同的脚本
		template<typename T>
		void Bind()
		{
			InstantiateFunction = [&]() { Instance = new T(); };
			DestroyInstanceFunction = [&]() { delete (T*)Instance; Instance = nullptr; };

			//OnCreateFunction = [](ScriptableEntity* instance) { ((T*)instance)->OnCreate(); };
			//OnDestroyFunction = [](ScriptableEntity* instance) { ((T*)instance)->OnDestroy(); };
			//OnUpdateFunction = [](ScriptableEntity* instance, Timestep ts) { ((T*)instance)->OnUpdate(ts); };
		
			OnCreateFunction = [this]() { ((T*)Instance)->OnCreate(); };
			OnDestroyFunction = [this]() { ((T*)Instance)->OnDestroy(); };
			OnUpdateFunction = [this](Timestep ts) { ((T*)Instance)->OnUpdate(ts); };
		}
	};

}