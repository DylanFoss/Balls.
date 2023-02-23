#include "PhysicsObject.h"

#include "Engine/Physics/Body.h"
#include "Engine/Physics/Collision/Collider.h"

//#include "Engine/Physics/Transform.h"

PhysicsObject::PhysicsObject(Collider* collider, const BodyDefinition& body)
{
	m_Body = new Body(body);
	m_Collider = collider;
	m_Body->UpdateMass(m_Collider->CalculateMass());
}

PhysicsObject::~PhysicsObject()
{
	delete m_Body;
	delete m_Collider;
}

void PhysicsObject::Update(float deltaTime)
{
	m_Body->Update(deltaTime);
}

const Transform* PhysicsObject::GetTransform() const
{
	return m_Body->GetTransform();
}

glm::vec2 PhysicsObject::GetPosition() const
{
	return m_Body->GetPosition();
}

glm::vec2 PhysicsObject::GetVelocity() const
{
	return m_Body->GetVelocity();
}

void PhysicsObject::OffsetPosition(const glm::vec2& pos)
{
	m_Body->OffsetPosition(pos);
}

void PhysicsObject::SetPosition(const glm::vec2& pos)
{
	m_Body->SetPosition(pos);
}

void PhysicsObject::Accelerate(const glm::vec2& acc)
{
	m_Body->Accelerate(acc);
}

float PhysicsObject::GetMass()
{
	return m_Body->GetMass();
}

float PhysicsObject::GetInverseMass()
{
	return m_Body->GetInverseMass();
}
