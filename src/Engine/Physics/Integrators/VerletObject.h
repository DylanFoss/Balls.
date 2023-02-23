#include "Engine/Physics/Integrators/IntegratorObject.h"

/*
* Verlet integration, where the current and old position are used to aproximate velocity.
*/
struct VerletObject
	: public IntegratorObject
{
	VerletObject(Transform* transform)
		: m_Transform(transform), m_PositionOld(transform->m_Position), m_Acceleration({ 0., 0. })
	{};

	Transform* m_Transform;
	glm::vec2 m_PositionOld;
	glm::vec2 m_Acceleration;

	virtual void UpdatePosition(float deltaTime) override
	{
		glm::vec2 Velocity = m_Transform->m_Position - m_PositionOld;
		m_PositionOld = m_Transform->m_Position;
		m_Transform->m_Position = m_Transform->m_Position + Velocity + m_Acceleration * deltaTime * deltaTime;
		m_Acceleration = { 0.0f, 0.0f };
	}

	virtual glm::vec2 GetPosition() const override { return m_Transform->m_Position; }
	virtual glm::vec2 GetAcceleration() const override { return m_Acceleration; }
	virtual glm::vec2 GetVelocity() const override { return m_Transform->m_Position - m_PositionOld; }

	virtual void Accelerate(const glm::vec2& acc) override { m_Acceleration += acc; }
	virtual void OffsetPosition(const glm::vec2& pos) override { m_Transform->m_Position += pos; }
	virtual void SetPosition(const glm::vec2& pos) override { m_Transform->m_Position = pos; }
	virtual void SetVelocity(const glm::vec2& vel) override { m_PositionOld = vel + m_Transform->m_Position; }

	virtual void SetTransform(Transform& transform) { m_Transform = &transform; }

	virtual VerletObject* Clone() const override
	{
		return new VerletObject(*this);
	}
};