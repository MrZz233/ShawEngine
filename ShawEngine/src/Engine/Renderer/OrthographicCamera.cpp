#include "sepch.h"
#include "Engine/Renderer/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace ShawEngine {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		SE_PROFILE_FUNCTION();
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		SE_PROFILE_FUNCTION();
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		//求逆
		//glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1)) * glm::translate(glm::mat4(1.0f), m_Position);
		//m_ViewMatrix = glm::inverse(transform);
		
		//不用求逆
		SE_PROFILE_FUNCTION();
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), -m_Position);
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		rotate = glm::transpose(rotate);

		//物体先平移后旋转 == 摄像机先旋转后再平移。
		//即xy方向会跟着摄像机变
		//物体围绕摄像机作旋转
		m_ViewMatrix = rotate * trans ;		//适合第一人称

		//摄像机先平移后旋转
		//即摄像机的xy固定在了世界的xy，
		//物体围绕原点作旋转
		//m_ViewMatrix = trans * rotate ;	//适合锁定敌人视角

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
