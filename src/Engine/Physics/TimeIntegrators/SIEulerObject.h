#include "Engine/Physics/TimeIntegrators/IntegratorObject.h"
#include "glm/glm.hpp"

struct SIEulerObject
	: public IntegratorObject
{
	glm::vec2 m_Position;
	glm::vec2 m_Acceleration;
	glm::vec2 m_Velocity;

	virtual void UpdatePosition(float deltaTime) override
	{
		m_Velocity += m_Acceleration * deltaTime;
		m_Position += m_Velocity * deltaTime;
		m_Acceleration = { 0.0f, 0.0f };
	}
};