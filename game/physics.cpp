#include "physics.h"

CPhysicsObject::CPhysicsObject(CTransform& transform) : m_transform(transform)
{
	m_acceleration = { 0,0,0 };
	m_velocity     = { 0,0,0 };
	m_resistance   = { 0,0,0 };

}

void CPhysicsObject::Simulate(float dt)
{
	m_transform.SetLocalOrigin(m_transform.GetLocalOrigin() + m_velocity * dt);
	m_velocity -= m_velocity * m_resistance * dt;
	m_velocity += m_acceleration * dt;
}
