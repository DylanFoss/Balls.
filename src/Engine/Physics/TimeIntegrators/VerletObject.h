#include "Engine/Physics/TimeIntegrators/IntegratorObject.h"
#include "glm/glm.hpp"

struct VerletObject
	: public IntegratorObject
{
	glm::vec2 m_Position;
	glm::vec2 m_PositionOld;
	glm::vec2 m_Acceleration;

	virtual void UpdatePosition(float deltaTime) override
	{
		glm::vec2 Velocity = m_Position - m_PositionOld;
		m_PositionOld = m_Position;
		m_Position = m_Position + Velocity + m_Acceleration * deltaTime * deltaTime;
		m_Acceleration = { 0.0f, 0.0f };
	}
};