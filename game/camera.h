#include "basesystem.h"
#include <glm/glm/ext/vector_float3.hpp>

class CTransform;

DECLARE_SINGLETON_ACCESSOR(Camera);
class CCamera : public IBaseSingleton<CCamera>
{
	DECLARE_SINGLETON(Camera);
public:
	void Setup(CTransform* target, glm::vec3 offset);
	void Update();

	// Use this to get an angle that looks directly at the camera
	glm::vec3 GetLookAtCamAng();

private:
	CTransform* m_target;
	glm::vec3   m_offset;
	glm::vec3   m_lookAtMe;
};