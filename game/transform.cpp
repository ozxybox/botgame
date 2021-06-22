#include "transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

CTransform::CTransform()
{
	m_position = { 0.0f,0.0f,0.0f };
	m_angles = { 0.0f,0.0f,0.0f };
	m_scale = { 1.0f,1.0f,1.0f };
	m_parent = nullptr;

}

void CTransform::SetParent(CTransform* parent)
{
	if (parent != this)
		m_parent = parent;
}

void CTransform::SetAbsAngles(glm::vec3 ang)
{
	for (CTransform* p = m_parent; p; p = p->m_parent)
	{
		ang -= p->m_angles;
	}

	m_angles = ang;
}

void CTransform::SetAbsOrigin(glm::vec3 pos)
{
	for (CTransform* p = m_parent; p; p = p->m_parent)
	{
		pos -= p->m_position;
	}

	m_position = pos;
}

void CTransform::SetAbsScale(glm::vec3 scale)
{
	for (CTransform* p = m_parent; p; p = p->m_parent)
	{
		scale /= p->m_scale;
	}

	m_scale = scale;
}

glm::vec3 CTransform::GetAbsAngles()
{
	glm::vec3 ang = m_angles;
	for (CTransform* p = m_parent; p; p = p->m_parent)
	{
		ang += p->m_angles;
	}

	return ang;
}

glm::vec3 CTransform::GetAbsOrigin()
{
	glm::vec3 pos = m_position;
	for (CTransform* p = m_parent; p; p = p->m_parent)
	{
		pos += p->m_position;
	}

	return pos;
}

glm::vec3 CTransform::GetAbsScale()
{
	glm::vec3 scale = m_scale;
	for (CTransform* p = m_parent; p; p = p->m_parent)
	{
		scale *= p->m_scale;
	}

	return scale;
}

glm::mat4 CTransform::Matrix()
{
	glm::mat4 mtx = glm::identity<glm::mat4>();
	if (m_parent)
		mtx = m_parent->Matrix();
	mtx = glm::translate(mtx, m_position);
	mtx *= glm::yawPitchRoll(m_angles.y, m_angles.x, m_angles.z);
	mtx = glm::scale(mtx, m_scale);

	return mtx;
}
