#include "sepch.h"
#include "Engine/Scene/Entity.h"

namespace ShawEngine {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

}