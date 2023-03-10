#pragma once
#include "glm/glm.hpp"

struct IntegratorObject;
struct Transform;

struct BodyDefinition
{
	glm::vec2 m_Position = { 0.0f, 0.0f };
	float m_Mass = 0.0f;
	float m_InvMass = 0.0f;
};

struct MassData
{
	MassData(float mass = 0.0f, float invMass = 0.0f): m_Mass(mass), m_InvMass(invMass) {};

	float m_Mass;
	float m_InvMass;

	//not used, will be needed for rotations
	//float m_Inertia
	//float m_InvInertia
};

class Body
{
public:
	Body(const BodyDefinition& BodyDef, Transform& transform);
	Body(const Body& other);
	Body operator=(const Body& other);

	Body(Body&& other) noexcept;
	Body operator=(Body&& other) noexcept;

	~Body();

	glm::vec2 GetPosition() const;
	glm::vec2 GetAcceleration() const;
	glm::vec2 GetVelocity() const;

	void Accelerate(const glm::vec2& acc);
	void OffsetPosition(const glm::vec2& pos);
	void SetPosition(const glm::vec2& pos);
	void SetVelocity(const glm::vec2& vel);

	void SetTransform(Transform& transform);

	void Update(float deltaTime);

	void UpdateMass(float mass);

	float GetMass() { return m_MassData.m_Mass; }
	float GetInverseMass() { return m_MassData.m_InvMass; }

private:
	IntegratorObject* m_Object;
	MassData m_MassData;
};