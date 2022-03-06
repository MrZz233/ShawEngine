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
		//����
		//glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1)) * glm::translate(glm::mat4(1.0f), m_Position);
		//m_ViewMatrix = glm::inverse(transform);
		
		//��������
		SE_PROFILE_FUNCTION();
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), -m_Position);
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		rotate = glm::transpose(rotate);

		//������ƽ�ƺ���ת == ���������ת����ƽ�ơ�
		//��xy���������������
		//����Χ�����������ת
		m_ViewMatrix = rotate * trans ;		//�ʺϵ�һ�˳�

		//�������ƽ�ƺ���ת
		//���������xy�̶����������xy��
		//����Χ��ԭ������ת
		//m_ViewMatrix = trans * rotate ;	//�ʺ����������ӽ�

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
