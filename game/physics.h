#pragma once
#include "transform.h"

class CPhysicsObject
{
public:
	CPhysicsObject(CTransform& transform);

	void Simulate(float dt);
	void SetVelocity(glm::vec3 vel) { m_velocity = vel; }
	void SetResistance(glm::vec3 res) { m_resistance = res; };
	void SetAcceleration(glm::vec3 acc) { m_acceleration = acc; }

private:
	CTransform& m_transform;
	glm::vec3  m_velocity;
	glm::vec3  m_acceleration;
	// Values from 0-1 to degrade our velocity
	glm::vec3  m_resistance;
};