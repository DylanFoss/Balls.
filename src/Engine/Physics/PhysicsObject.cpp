#include "PhysicsObject.h"

#include "Engine/Physics/Body.h"
#include "Engine/Physics/Collision/Collider.h"

PhysicsObject::PhysicsObject(Collider* shape, const BodyDefinition& body)
{
	m_Body = new Body(body);
	m_Shape = shape;
	m_Body->UpdateMass(m_Shape->CalculateMass());
}

PhysicsObject::~PhysicsObject()
{
	delete m_Body;
	delete m_Shape;
}

void PhysicsObject::Update(float deltaTime)
{
	m_Body->Update(deltaTime);
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
