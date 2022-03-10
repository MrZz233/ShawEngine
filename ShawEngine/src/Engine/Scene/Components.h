#pragma once
#include "Engine/Scene/SceneCamera.h"
#include "Engine/Scene/ScriptableEntity.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), { 1, 0, 0 })
				*glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), { 0, 1, 0 })
				*glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), { 0, 0, 1 });

			return glm::translate(glm::mat4(1.0f), Translation)
				* glm::mat4(rotation)
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		glm::mat4 GetCamTransform() const
		{
			glm::vec3 front;
			float yaw = Rotation.y;
			float pitch = Rotation.x;
			front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			front.y = sin(glm::radians(pitch));
			front.z = -sin(glm::radians(yaw)) * cos(glm::radians(pitch));

			glm::vec3 Front = -glm::normalize(front);
			// also re-calculate the Right and Up vector
			glm::vec3 Right = glm::normalize(glm::cross({ 0,1,0 },Front));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			glm::vec3 Up = glm::normalize(glm::cross(Front,Right));

			glm::mat3 rotation;
			rotation[0] = Right; rotation[1] = Up; rotation[2] = Front;

			return glm::translate(glm::mat4(1.0f), Translation)
				* glm::mat4(rotation)
				* glm::scale(glm::mat4(1.0f), Scale);
		}

	};

	enum class Shape
	{
		Quad,
		Box
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Shape _shape{Shape::Quad};

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color,Shape shape)
			: Color(color), _shape(shape) {}
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

		ScriptableEntity* (*InstantiateScript)();
		std::function<void()> DestroyScript;


		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [&]() { delete Instance; Instance = nullptr; };
		}
	};

}