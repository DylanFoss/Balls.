#include "Engine/Physics/Integrators/IntegratorObject.h"

/*
* Semi-Implict Euler integration, where the n+1 velocity is used to calculate the n+1 position.
*/
struct SIEulerObject
	: public IntegratorObject
{
	SIEulerObject(glm::vec2& position)
		: m_Position(position), m_Acceleration({ 0.f, 0.f }), m_Velocity(glm::vec2 { 0.f, 0.f })
	{
	};

	glm::vec2& m_Position;
	glm::vec2 m_Acceleration;
	glm::vec2 m_Velocity;

	virtual void UpdatePosition(float deltaTime) override
	{
		m_Velocity += m_Acceleration * deltaTime;
		m_Position += m_Velocity * deltaTime;
		m_Acceleration = { 0.0f, 0.0f };
	}

	virtual glm::vec2 GetPosition() const override { return m_Position; };
	virtual glm::vec2 GetAcceleration() const override { return m_Acceleration; };
	virtual glm::vec2 GetVelocity() const override { return m_Velocity; };

	virtual void Accelerate(const glm::vec2& acc) override { m_Acceleration += acc; }
	virtual void OffsetPosition(const glm::vec2& pos) override { m_Position += pos; }
	virtual void SetPosition(const glm::vec2& pos) override { m_Position = pos; }
};