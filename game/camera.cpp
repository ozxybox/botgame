#include "camera.h"
#include "utils.h"
#include <renderer.h>
#include <transform.h>
#include <glm/glm/ext/matrix_transform.hpp>

DEFINE_SINGLETON(Camera);


void CCamera::Setup(CTransform* target, glm::vec3 offset) 
{
	m_offset = offset;
	m_target = target;
	Angles(glm::normalize(-offset), m_lookAtMe);
}

void CCamera::Update()
{

	Renderer().SetPerspective(glm::radians(45.0f));
	glm::vec3 pos = m_target->GetAbsOrigin();
	glm::mat4x4 m = glm::lookAt(pos + m_offset, pos, { 0,1,0 });
	Renderer().SetMatrix(MatrixMode::VIEW, m);
}

glm::vec3 CCamera::GetLookAtCamAng()
{
	return m_lookAtMe;
}